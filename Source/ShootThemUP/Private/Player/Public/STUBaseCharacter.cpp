// Shoo Them Up Game


#include "Player/Public/STUBaseCharacter.h"
#include "Components/STUCharacterMovementComponent.h" // ���������� ��������������� ��������� ��� ������ character
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h" // ��� ������������ ����� Pawn

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer &ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{// ������� ����������� ������������� ������. ������� SetDefaultSubobjectClass ��� ������ ���������� �� ���������
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent"); // ���� ��������� ����� ���������� ������� ��� �� ����� SetupAttachment
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent"); // ���� ��������� ����� ���������� ������� ��� �� ����� SetupAttachment
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    check(HealthComponent); // �������� �� nullptr ����� ������ check - ���������� Assertion, �������� ������ � Developend � Debug ������
    check(GetCharacterMovement());
    check(GetMesh());

    OnHealthChanged(HealthComponent->GetHealth(), 0.0f); // ���� ������� ����� ������� OnHealthChanged �� ��������� ������� �� ������� ����� GetHealth
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath); // ����������� �� �������. ���� AddUObject(��� ������, ��������� �� �������) ������ ���� ��� ������� �������� � �++
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded); // ������������� �� ������� LandedDelegate - ������� ���������� ��� �������� ��������� � ������������ ����� �������

}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASTUBaseCharacter::IsRunning() const
{
    return false; // �������� ��� ������ ������ � ��� �������� ��������� �����, � ��� �������� ��������� �� ����� 0 + �������� �� ��������� � �������
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal(); // ��������� ������� ������ ������� ��������
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal)); // ����������� ��������� ������������ ����� Forward � �������� ������� VelocityNormal
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal); // ����������� ������������� ������
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);  // �������� AngleBetween � ������� � �������� �� ���� ���������� Z CrossProduct
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::OnDeath()
{
    GetCharacterMovement()->DisableMovement(); // ������������� �� �������� �������� ������� DisableMovement

    SetLifeSpan(LifeSpanOnDeath); // ����������� ������ ���������

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // ������ ������������� ������� ��� ������ ���������
    WeaponComponent->StopFire(); // ������������� �������� ��� ������ ���������
    // ��������� ������ ��������� ������ ����� �������� DeathAnimMontage
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ������� ������� �������� ������� ���������� SetCollisionEnabled()
    GetMesh()->SetSimulatePhysics(true); // ������� SetSimulatePhysics() - ���������� ���������� ��������� ����
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult &Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z; // ����� ��� �������� ������������� � ������������, ��� ��� �������� �������� �������������������� ������ � ������� ������ ���� �����

    if (FallVelocityZ < LandedDamageVelocity.X) // �������� ���� �������� ������� ������ 900, ������� �� �������
    {
        return;
    }

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ); // ���������� ������. ������� GetMappedRangeValueClamped(������� �������, ����, �������� � ������� �����)
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr); // ��������� �����
}

void ASTUBaseCharacter::OnStartFire()
{
    if (IsRunning()) // ���� �������� �������� 
    {
        return; // �������
    }
    WeaponComponent->StartFire(); // ���� ���, �������� ������� StartFire
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
    const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0); // ��������� �� ���������������
    if (!MaterialInst) // ���� ��������������� �� ����������
    {
        return; // ����� �� �������
    }

    MaterialInst->SetVectorParameterValue(MaterialColorName, Color); // ������������� ���� ���������
}
