// Shoo Them Up Game


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h" // ��� ����� � �����
#include "Weapon/Components/STUWeaponFXComponent.h" // ���� ������ ���������� �������
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent"); // ������ ���������
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent); // �������� ��� ��� ��������� ����������
}

void ASTURifleWeapon::StartFire() // ������� ����
{
    //UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"))
    InitMuzzleFX(); // ������ ��� ����� �� �������� �������� ������� ������ ������� ���� InitMuzzleFX() - ��������� ������ 1 ���

    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASTURifleWeapon::StopFire() // ������� ����
{
    //UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"))

    GetWorldTimerManager().ClearTimer(ShotTimerHandle); // ������������� ������
    SetMuzzleFXVisibility(false); // ����������� ��������, ������������� ������� ����
}

void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty()) // �������� �� ������������� ���� ��� ������� ������
    {
        StopFire(); // ��������� ��������
        return;
    }

    FVector TraceStart; // ������ �����
    FVector TraceEnd;   // ����� �����
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire(); // ��������� ��������
        return;
    }

    FHitResult HitResult; // ��� ���������� � ������������
    MakeHit(HitResult, TraceStart, TraceEnd);

    FVector TraceFXEnd = TraceEnd; // ���������� ����� ����� ������� ������
    if (HitResult.bBlockingHit) // �������� �� ����������� ����
    {
        TraceFXEnd = HitResult.ImpactPoint;
        MakeDamage(HitResult);
       // DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f); // ������ ����� �������� DrawDebugLine(���, ������ �����, ����� �����, ����, ����� �����������
                   // ������� ����, ������� ������� ����� ���������� �����, ������� ��������� �������, �������)
       // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f); // ���������� ����� � ����� �����������

        WeaponFXComponent->PlayImpactFX(HitResult); // �������� ��� ������ �������

        // UE_LOG(LogBaseWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString()); // ����� � ������� �����, �
        // ������� �� ������. ���������� � ����� ��������� � BoneName
    }
    /*else // ���� �� ������ �� ������
    {
        //DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }*/
    SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd); // ����� ������ (������� ����, ����� ������)
    DecreaseAmmo(); // ���� ������� ������� ��������� ���-�� ��������
}

bool ASTURifleWeapon::GetTraceData(FVector &TraceStart, FVector &TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    TraceStart = ViewLocation; // ����� ������ ��������
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread); // ���������� � �������� BulletSpread - �������� �������� ���� ������
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad); // ����������� �������� �� ������. VRandCone(����������� ������, ���� ��� �������)
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance; // �������� ����� ��������
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
    if (!MuzzleFXComponent) // �������� �� ��������� ����������, ���� ������ ��� �� ����
    {
        MuzzleFXComponent = SpawnMuzzleFX(); // ������� �������
    }
    SetMuzzleFXVisibility(true); // ��������� �������
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible)
{
    if (MuzzleFXComponent) // �������� ��� ������� ����������
    {
        MuzzleFXComponent->SetPaused(!Visible); // �������� ������� SetPaused(��������� ��������) - ������������� ���� ��������� ������ �������
        MuzzleFXComponent->SetVisibility(Visible, true); // �������� ������� SetVisibility(�������� visible, �������� ���������(� ����� ������ ��� �����, � ��� ��� ����)) - �������� �� ������ �������
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector &TraceStart, const FVector &TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart); // SpawnSystemAtLocation(���, ��� �����, ������� ������� � ����)
    if (TraceFXComponent) // ���� ��� ������ �����������
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd); // SetNiagaraVariableVec3(��� ����������, ����� ������)
    }
}

AController* ASTURifleWeapon::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr; // �������� �� ��������� GetOwner ��������-�� �� �����������, ���� �������� ����� ��� ����������
}
