// Shoo Them Up Game


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h" // для сферы и линии
#include "Weapon/Components/STUWeaponFXComponent.h" // файл нашего компонента ниагары
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent"); // создаём компонент
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent); // проверка что наш компонент существует
}

void ASTURifleWeapon::StartFire() // функция огня
{
    //UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"))
    InitMuzzleFX(); // каждый раз когда мы стреляем вызываем функцию спавна вспышки дула InitMuzzleFX() - спавнится только 1 раз

    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASTURifleWeapon::StopFire() // функция огня
{
    //UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"))

    GetWorldTimerManager().ClearTimer(ShotTimerHandle); // останавливаем таймер
    SetMuzzleFXVisibility(false); // заканчиваем стрельбу, останавливаем вспышку дула
}

void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty()) // проверка на существования мира или арсенал пустой
    {
        StopFire(); // запрещаем стрельбу
        return;
    }

    FVector TraceStart; // начало линии
    FVector TraceEnd;   // конец линии
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire(); // запрещаем стрельбу
        return;
    }

    FHitResult HitResult; // вся информация о пересечениях
    MakeHit(HitResult, TraceStart, TraceEnd);

    FVector TraceFXEnd = TraceEnd; // переменная конца точки эффекта трейса
    if (HitResult.bBlockingHit) // проверка на пересечение лини
    {
        TraceFXEnd = HitResult.ImpactPoint;
        MakeDamage(HitResult);
       // DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f); // рисуем линию выстрела DrawDebugLine(мир, начало линии, конец линии, цвет, время отображения
                   // рисовки лини, секунды сколько будет рисоваться линия, порядок отрисовки глубины, толщина)
       // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f); // нарисовали сферу в точки пересечения

        WeaponFXComponent->PlayImpactFX(HitResult); // вызывает наш эффект Ниагару

        // UE_LOG(LogBaseWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString()); // вывод в консоль кости, в
        // которую мы попали. Информация о кости находится в BoneName
    }
    /*else // если мы никуда не попали
    {
        //DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }*/
    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd); // спавн трейса (позиция дула, конец трейса)
    DecreaseAmmo(); // если сделали выстрел уменьшаем кол-во патронов
}

bool ASTURifleWeapon::GetTraceData(FVector &TraceStart, FVector &TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    TraceStart = ViewLocation; // точка начала выстрела
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread); // переменная в радианах BulletSpread - хранится половина угла конуса
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad); // направление стрельбы из оружия. VRandCone(направление конуса, угол при вершине)
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance; // конечная точка выстрела
    return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult &HitResult)
{
    const auto DamageActor = HitResult.GetActor();
    if (!DamageActor)
    {
        return;
    }

    DamageActor->TakeDamage(DamageAmout, FDamageEvent(), GetController(), this);
}

void ASTURifleWeapon::InitMuzzleFX()
{
    if (!MuzzleFXComponent) // проверка на указатель компонента, если спавна ещё не было
    {
        MuzzleFXComponent = SpawnMuzzleFX(); // спавним систему
    }
    SetMuzzleFXVisibility(true); // видимость ниагары
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible)
{
    if (MuzzleFXComponent) // проверка что ниагара заспавнена
    {
        MuzzleFXComponent->SetPaused(!Visible); // вызываем фукнцию SetPaused(отрицание значение) - останавливаем либо запускает ниагар систему
        MuzzleFXComponent->SetVisibility(Visible, true); // вызываем функцию SetVisibility(параметр visible, дочерний компонент(в нашем случае все равно, у нас его нету)) - отвечает за рендер системы
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector &TraceStart, const FVector &TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart); // SpawnSystemAtLocation(мир, наш трейс, локация системы в мире)
    if (TraceFXComponent) // если наш эффект заспавнился
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd); // SetNiagaraVariableVec3(имя переменной, конец трейса)
    }
}

AController* ASTURifleWeapon::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr; // проверка на указатель GetOwner являится-ли он котроллером, если является вернём его контроллер
}
