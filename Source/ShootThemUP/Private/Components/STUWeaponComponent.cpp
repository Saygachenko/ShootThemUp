// Shoo Them Up Game


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h" // библиотека персонажа
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

    checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);

    InitAnimations();
	SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr; // CurrentWeapon = 0
    for (auto Weapon : Weapons) // пробегаемся по массиву оружия
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); // отсоединяем данного актора от меша
        Weapon->Destroy(); // удаляем актора оружия
    }
    Weapons.Empty(); // проверка есть-ли элементы в массиве

    Super::EndPlay(EndPlayReason); // вызов EndPlay
}

void USTUWeaponComponent::SpawnWeapons()
{

    ACharacter *Character = Cast<ACharacter>(GetOwner()); // указатель на GetOwner()
    if (!Character || !GetWorld()) // проверка на Owner и мир игры
    {
        return;
    }

    for (auto OneWeaponData : WeaponData) // проходимся по всему массиву WeaponData
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass); // спавним оружие в мире игры. Функция SpawnActor возвращает указатель на созданный объект
        if (!Weapon) // если оружие заспавнено
        {
            continue;
        }
        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
        Weapon->SetOwner(Character); // указываем владельца нашего оружия
        Weapons.Add(Weapon); // добавляем в Weapons указатель на владельца

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent) 
    {
        return;
    }
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false); // AttachmentRules(EAttachmentRule, симулияци физических тел)
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName); // присоединяем оружие к сокету.
                                                            // AttachToComponent(указатель на компонент который мы хотим присоединить к данному персонажу, AttachmentRules, название сокента скелета)
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num()) // проверка на индекс оружия, должен быть больше нуля либо меньше кол-во элементов массива
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
    }

    ACharacter *Character = Cast<ACharacter>(GetOwner()); // указатель на GetOwner()
    if (!Character)
    {
        return;
    }
    if (CurrentWeapon) // CurrentWeapon будет нулевым только при вызове в BeginPlay
    {
        CurrentWeapon->StopFire(); // останавливаем стрельбу при смене оружия
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName); // возвращает оружие за спину персонажа
    }

    CurrentWeapon = Weapons[WeaponIndex]; // индекс текущего оружия
                                                                                               // параметр функции Data имеет тип элемента массива
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData &Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); }); // каждый раз будет искать в массиве WeaponData структуру которая соответствует классу нашего оружия и если находим берём от туда анимацию перезарядки
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr; // проверка на 0

    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName); // прикрепляем оружие в руки персонажа

    EquipAnimInProgress = true;

    PlayAnimMontag(EquipAnimMontage); // вызывает анимацию EquipAnimMontage, чтобы она проигрывалась при смене оружия
}

void USTUWeaponComponent::StartFire() // функция огня
{
    if (!CanFire()) // если мы не можем стрелять, то выходим из функции
    {
        return;
    }
    CurrentWeapon->StartFire(); // если есть, вызываем функцию Fire
}

void USTUWeaponComponent::StopFire() // функция огня
{
    if (!CurrentWeapon) // если кнопка отжата, то выходим из фукнции
    {
        return;
    }
    CurrentWeapon->StopFire(); // если есть, вызываем функцию Fire
}

void USTUWeaponComponent::NextWeapon()
{
    if (!CanEquip()) // если мы не можем менять оружие, то просто выходим из фукнции
    {
        return;
    }
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num(); // увеличиваем CurrentWeaponIndex на 1 и чтобы переменная не вышла за пределы массива Wepons возьмём его по модулю длины массива Wepons.Num - вычисляем количество элементов в массиве
    EquipWeapon(CurrentWeaponIndex); // вызываем EquipWeapon
}

void USTUWeaponComponent::PlayAnimMontag(UAnimMontage *Animation)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner()); // указатель на GetOwner()
    if (!Character)
    {
        return;
    }

    Character->PlayAnimMontage(Animation); // вызываем функцию PlayAnimMontage
}

void USTUWeaponComponent::InitAnimations()
{
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify) // если это наш Notify то
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEnquipFinished); // достаём наш делегат и биндим с помощью функции AddUObject нашу функцию OnEnquipFinished
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
        checkNoEntry();
    }

    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishedNotify) // если мы не нашли Notify то выходим и продолжаем поиск
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
            checkNoEntry();
        }
        ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished); // достаём наш делегат и биндим с помощью функции AddUObject нашу функцию OnEnquipFinished
    }
}

void USTUWeaponComponent::OnEnquipFinished(USkeletalMeshComponent *MeshComp)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner()); // указатель на GetOwner()
    if (!Character || MeshComp != Character->GetMesh()) // проверка что нашего Character есть владелец, и что наш скелет совпадает со скелетом который пришол в MeshComponent (чтобы только мы подписались на этот делегает а не все персонажи)
    {
        return;
    }
    EquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent *MeshComp)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner()); // указатель на GetOwner()
    if (!Character || MeshComp != Character->GetMesh()) // проверка что нашего Character есть владелец, и что наш скелет
                                                        // совпадает со скелетом который пришол в MeshComponent (чтобы
                                                        // только мы подписались на этот делегает а не все персонажи)
    {
        return;
    }
    ReloadAnimInProgress = false;
}

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::Reload()
{
    ChangeClip();
}

void USTUWeaponComponent::OnEmptyClip(ASTUBaseWeapon *AmmoEmptyWeapon)
{
    if (!AmmoEmptyWeapon) // если указатель 0
    {
        return; // выход из фукнции
    }
    if (CurrentWeapon == AmmoEmptyWeapon) // если текущее оружие равняется AmmoEmptyWeapon
    {
        ChangeClip(); // вызываем анимацию перезарядки
    }
    else // если оружие другое
    {
        for (const auto Weapon : Weapons) // пробегаемся по массиву оружия
        {
            Weapon->ChangeClip(); // и если мы такое оружие находим то просто молча вызываем ChangeClip данного оружия (анимация проигрываться не будет, но логика перезарядки будет)
        }
    }
}

void USTUWeaponComponent::ChangeClip()
{
    if (!CanReload())
    {
        return;
    }
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();
    ReloadAnimInProgress = true;
    PlayAnimMontag(CurrentReloadAnimMontage); // проигрывание анимации перезарядки
}

bool USTUWeaponComponent::IsFiring() const
{
    return CurrentWeapon && CurrentWeapon->IsFiring(); // возвращает true оружие есть и стрельба началась
}

bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData &UIData) const
{
    if (CurrentWeapon) // если оружие есть
    {
        UIData = CurrentWeapon->GetUIData(); // возвращаем текущему оружия UIData
        return true;
    }
    return false;
}

bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData &AmmoData) const
{
    if (CurrentWeapon) // если оружие есть
    {
        AmmoData = CurrentWeapon->GetAmmoData(); // возвращаем текущий арсенал
        return true;
    }
    return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
    for (const auto Weapon : Weapons) // пробегаем по массиву оружия
    {
        if (Weapon && Weapon->IsA(WeaponType)) // проверяем указатель Weapon на 0 и существует-ли данный тип оружия
        {
            return Weapon->TryToAddAmmo(ClipsAmount); // если условие выполнено , вызываем функцию TryToAddAmmo(кол-во магазинов)
        }
    }
    return false; // выход из функции
}

bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType)
{
    for (const auto Weapon : Weapons) // пробегаем по массиву оружия
    {
        if (Weapon && Weapon->IsA(WeaponType)) // проверяем указатель Weapon на 0 и существует-ли данный тип оружия
        {
            return !Weapon->IsAmmoFull(); // если условие выполнено , вызываем функцию IsAmmoFull() - пополняется наш боезапас
        }
    }
    return false; // выход из функции
}

