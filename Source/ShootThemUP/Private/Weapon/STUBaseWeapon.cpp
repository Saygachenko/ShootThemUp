// Shoo Them Up Game


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h" // для сферы и линии
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh); // делаем его коренным

}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

    check(WeaponMesh); // проверка
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullet count could't be less or equal zero")); // работает так же как и check, только можно ещё добавить пользовательское сообщение об ошибке
    checkf(DefaultAmmo.Clips > 0, TEXT("Bullet count could't be less or equal zero"));

    CurrentAmmo = DefaultAmmo; // инициализация CurrentAmmo = DefaultAmmo
	
}

void ASTUBaseWeapon::StartFire() // функция огня
{
    // UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"))
    FireInProgress = true;
}

void ASTUBaseWeapon::StopFire() // функция огня
{
    // UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"))
    FireInProgress = false;
}

void ASTUBaseWeapon::MakeShot()
{

}

/*APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner()); // указатель на владельца
    if (!Player)                                      // проверка на владельца
    {
        return nullptr;
    }

    return Player->GetController<APlayerController>(); // указатель на контроллер
}*/

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto STUCharacter = Cast<ACharacter>(GetOwner()); // каст к чарактеру GetOwner
    if (!STUCharacter) // если объект 0
    {
        return false; // выход из фукнции
    }

    if (STUCharacter->IsPlayerControlled()) // проверяем, контроллируется данный Character AI или человеком. IsPlayerControlled - Функция возвращает bool значение 
    {
        const auto Controller = STUCharacter->GetController<APlayerController>(); // указатель на контроллер
        if (!Controller) // если контрля нет
        {
            return false; // выход из функции
        }

        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation); // фукнция которая возвращает позицию камеры и ориентацию в
                                                                    // пространстве GetPlayerViewPoint(положение камеры, вращение камеры)
    }
    else // если Character контроллирует AI (просто будем стрелять по направлению дула)
    {
        ViewLocation = GetMuzzleWorldLocation(); // присваиваем ViewLocation значении позиции нашего дула
        ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName); // ViewRotation записываем значение GetSocketRotation(MuzzleSocketName)
    }

    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector &TraceStart, FVector &TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    TraceStart = ViewLocation; // точка начала выстрела

    const FVector ShootDirection =  ViewRotation.Vector(); // направление стрельбы из оружия. VRandCone(направление конуса, угол при вершине)
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance; // конечная точка выстрела
    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
    if (!GetWorld())
    {
        return;
    }

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner()); // для игнорирования нашего персонажа
    CollisionParams.bReturnPhysicalMaterial = true; // физический материал по умолчанию не передаётся в параметр HitResult (для оптимизации), явно указываем чтобы физический материал попадал в HitResult

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams); // пересечение нащего отрезка с объектами которые находятся на сцене bReturnPhysicalMaterial = true;
                          // LineTraceSingleByChannel(вся информаци о пересечении, начальная точка линии, конечная точка
                          // линии, фильтрация всех объектов на сцене, инорирование нашего трейса)
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("Clips is empty!"));
        return;
    }
    CurrentAmmo.Bullets--; // уменьшаем количество патронов при выстреле
    if (IsClipEmpty() && !IsAmmoEmpty()) // если арсенал пустой и арсенал полностью не пустой
    {
        StopFire();
        OnClipEmpty.Broadcast(this); // вызываем функцию сменый обоймы
    }
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty(); // возвращает true когда арсенал полностью пустой (арсенал должен быть конечен, кол-во обойм = 0, кол-во патронов = 0)
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0; // возвращаем true когда обойма пустая
}

void ASTUBaseWeapon::ChangeClip()
{
    if (!CurrentAmmo.Infinite) // если кол-во патронов не бесконечно
    {
        if (CurrentAmmo.Clips == 0)
        {
            //UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips!"));
            return;
        }
        CurrentAmmo.Clips--; // уменьшаем кол-во обойм
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets; // пополняем обойму
    //UE_LOG(LogBaseWeapon, Display, TEXT("Change Clip")); // вызов в логи перезарядки
}

bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / "; // выводим количество патронов
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips); // выводим либо количество обойм, либо текст Infinite если арсенал бесконечные
    //UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo); // вывод в логи информации о арсенале
}


bool ASTUBaseWeapon::IsFiring() const
{
    return FireInProgress;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) // условие что мы не можем добавить пикап. Если оружие бесконечное или у нас полный боекомплект или кол-во магазинов меньше или = 0
    {
        return false; // выход из функции
    }
    if (IsAmmoEmpty()) // если кол-во магазинов и патронов 0
    {
        //UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty!"));
        CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1); // добавляем полный боекомплект (границы значений)
        OnClipEmpty.Broadcast(this); // вызываем наш делегат перезарядки
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips) // если кол-во обойм меньше дефолтного
    {
        const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount; // локальная переменная. Записываем в переменную сумму текущего кол-во магазинов + переданные кол-во магазинов
        if (DefaultAmmo.Clips - NextClipsAmount >= 0) // если значение разницы макс значения магазина и переменной NextClipsAmount больше или равно 0
        {
            CurrentAmmo.Clips = NextClipsAmount; // присваиваем текущему магазину NextClipsAmount
            //UE_LOG(LogBaseWeapon, Display, TEXT("Clips were added!"));
        }
        else // если разнима меньше 0
        {
            CurrentAmmo.Clips = DefaultAmmo.Clips; // пополняем обойму
            CurrentAmmo.Bullets = DefaultAmmo.Bullets; // пополняем патроны
            //UE_LOG(LogBaseWeapon, Display, TEXT("Ammo is fyll now!"));
        }
    }
    else // если кол-во обойм == дефолтному при этом мы знаем что наш арсенал не полон
    {
        CurrentAmmo.Bullets = DefaultAmmo.Bullets; // заполняем обойму
        //UE_LOG(LogBaseWeapon, Display, TEXT("Bullets were added!"));
    }
    return true; // произошло взятие пикапа
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets; // если кол-во патронов и магазинов полные возвращаем true
}

UNiagaraComponent *ASTUBaseWeapon::SpawnMuzzleFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, WeaponMesh, MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true); // SpawnSystemAttached(указатель на ниагару систему, компонент к которому мы хотим приатачить ниагар систему, название сокета, трансформация локации, трансформация поворота, аттач, автодестрой) - возвращает указатель на ниагару поэтому записали просто в return
}
