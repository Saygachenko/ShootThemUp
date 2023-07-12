// Shoo Them Up Game


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty()) // проверка на существования мира или арсенал пустой
    {
        return;
    }

    FVector TraceStart; // начало линии
    FVector TraceEnd;   // конец линии
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        return;
    }

    FHitResult HitResult; // вся информация о пересечениях
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd; // попали мы куда либо или нет
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal(); // высчитали вектор вдоль которого мы выпустили снаряд (конечная точка - начальная точка сделали его единичным)

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation()); // начальная трансформация нашей ракеты
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform); // указатель на актор ракеты BeginDeferredActorSpawnFromClass(мир, класс который заспанить хотим, начальная трансформация)
    if (Projectile) // если указатель на созданный актор не 0
    {
        Projectile->SetShotDirection(Direction); // устанавливаем направление выстрела
        Projectile->SetOwner(GetOwner()); // определяем хозяина актора
        Projectile->FinishSpawning(SpawnTransform); // завершаем спавн актора
    }
    DecreaseAmmo(); // если сделали выстрел уменьшаем кол-во патронов

    SpawnMuzzleFX(); // спавн ниагары вспышки дула
}
