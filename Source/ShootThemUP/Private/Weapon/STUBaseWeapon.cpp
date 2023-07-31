// Shoo Them Up Game


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h" // ��� ����� � �����
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
    SetRootComponent(WeaponMesh); // ������ ��� ��������

}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

    check(WeaponMesh); // ��������
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullet count could't be less or equal zero")); // �������� ��� �� ��� � check, ������ ����� ��� �������� ���������������� ��������� �� ������
    checkf(DefaultAmmo.Clips > 0, TEXT("Bullet count could't be less or equal zero"));

    CurrentAmmo = DefaultAmmo; // ������������� CurrentAmmo = DefaultAmmo
	
}

void ASTUBaseWeapon::StartFire() // ������� ����
{
    // UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"))
    FireInProgress = true;
}

void ASTUBaseWeapon::StopFire() // ������� ����
{
    // UE_LOG(LogBaseWeapon, Display, TEXT("Fire!"))
    FireInProgress = false;
}

void ASTUBaseWeapon::MakeShot()
{

}

/*APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner()); // ��������� �� ���������
    if (!Player)                                      // �������� �� ���������
    {
        return nullptr;
    }

    return Player->GetController<APlayerController>(); // ��������� �� ����������
}*/

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto STUCharacter = Cast<ACharacter>(GetOwner()); // ���� � ��������� GetOwner
    if (!STUCharacter) // ���� ������ 0
    {
        return false; // ����� �� �������
    }

    if (STUCharacter->IsPlayerControlled()) // ���������, ��������������� ������ Character AI ��� ���������. IsPlayerControlled - ������� ���������� bool �������� 
    {
        const auto Controller = STUCharacter->GetController<APlayerController>(); // ��������� �� ����������
        if (!Controller) // ���� ������� ���
        {
            return false; // ����� �� �������
        }

        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation); // ������� ������� ���������� ������� ������ � ���������� �
                                                                    // ������������ GetPlayerViewPoint(��������� ������, �������� ������)
    }
    else // ���� Character ������������� AI (������ ����� �������� �� ����������� ����)
    {
        ViewLocation = GetMuzzleWorldLocation(); // ����������� ViewLocation �������� ������� ������ ����
        ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName); // ViewRotation ���������� �������� GetSocketRotation(MuzzleSocketName)
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

    TraceStart = ViewLocation; // ����� ������ ��������

    const FVector ShootDirection =  ViewRotation.Vector(); // ����������� �������� �� ������. VRandCone(����������� ������, ���� ��� �������)
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance; // �������� ����� ��������
    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
    if (!GetWorld())
    {
        return;
    }

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner()); // ��� ������������� ������ ���������
    CollisionParams.bReturnPhysicalMaterial = true; // ���������� �������� �� ��������� �� ��������� � �������� HitResult (��� �����������), ���� ��������� ����� ���������� �������� ������� � HitResult

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams); // ����������� ������ ������� � ��������� ������� ��������� �� ����� bReturnPhysicalMaterial = true;
                          // LineTraceSingleByChannel(��� ��������� � �����������, ��������� ����� �����, �������� �����
                          // �����, ���������� ���� �������� �� �����, ������������ ������ ������)
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("Clips is empty!"));
        return;
    }
    CurrentAmmo.Bullets--; // ��������� ���������� �������� ��� ��������
    if (IsClipEmpty() && !IsAmmoEmpty()) // ���� ������� ������ � ������� ��������� �� ������
    {
        StopFire();
        OnClipEmpty.Broadcast(this); // �������� ������� ������ ������
    }
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty(); // ���������� true ����� ������� ��������� ������ (������� ������ ���� �������, ���-�� ����� = 0, ���-�� �������� = 0)
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0; // ���������� true ����� ������ ������
}

void ASTUBaseWeapon::ChangeClip()
{
    if (!CurrentAmmo.Infinite) // ���� ���-�� �������� �� ����������
    {
        if (CurrentAmmo.Clips == 0)
        {
            //UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips!"));
            return;
        }
        CurrentAmmo.Clips--; // ��������� ���-�� �����
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets; // ��������� ������
    //UE_LOG(LogBaseWeapon, Display, TEXT("Change Clip")); // ����� � ���� �����������
}

bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / "; // ������� ���������� ��������
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips); // ������� ���� ���������� �����, ���� ����� Infinite ���� ������� �����������
    //UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo); // ����� � ���� ���������� � ��������
}


bool ASTUBaseWeapon::IsFiring() const
{
    return FireInProgress;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) // ������� ��� �� �� ����� �������� �����. ���� ������ ����������� ��� � ��� ������ ����������� ��� ���-�� ��������� ������ ��� = 0
    {
        return false; // ����� �� �������
    }
    if (IsAmmoEmpty()) // ���� ���-�� ��������� � �������� 0
    {
        //UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty!"));
        CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1); // ��������� ������ ����������� (������� ��������)
        OnClipEmpty.Broadcast(this); // �������� ��� ������� �����������
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips) // ���� ���-�� ����� ������ ����������
    {
        const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount; // ��������� ����������. ���������� � ���������� ����� �������� ���-�� ��������� + ���������� ���-�� ���������
        if (DefaultAmmo.Clips - NextClipsAmount >= 0) // ���� �������� ������� ���� �������� �������� � ���������� NextClipsAmount ������ ��� ����� 0
        {
            CurrentAmmo.Clips = NextClipsAmount; // ����������� �������� �������� NextClipsAmount
            //UE_LOG(LogBaseWeapon, Display, TEXT("Clips were added!"));
        }
        else // ���� ������� ������ 0
        {
            CurrentAmmo.Clips = DefaultAmmo.Clips; // ��������� ������
            CurrentAmmo.Bullets = DefaultAmmo.Bullets; // ��������� �������
            //UE_LOG(LogBaseWeapon, Display, TEXT("Ammo is fyll now!"));
        }
    }
    else // ���� ���-�� ����� == ���������� ��� ���� �� ����� ��� ��� ������� �� �����
    {
        CurrentAmmo.Bullets = DefaultAmmo.Bullets; // ��������� ������
        //UE_LOG(LogBaseWeapon, Display, TEXT("Bullets were added!"));
    }
    return true; // ��������� ������ ������
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets; // ���� ���-�� �������� � ��������� ������ ���������� true
}

UNiagaraComponent *ASTUBaseWeapon::SpawnMuzzleFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, WeaponMesh, MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true); // SpawnSystemAttached(��������� �� ������� �������, ��������� � �������� �� ����� ���������� ������ �������, �������� ������, ������������� �������, ������������� ��������, �����, �����������) - ���������� ��������� �� ������� ������� �������� ������ � return
}
