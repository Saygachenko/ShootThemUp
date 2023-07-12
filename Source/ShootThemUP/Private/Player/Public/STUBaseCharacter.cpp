// Shoo Them Up Game


#include "Player/Public/STUBaseCharacter.h"
#include "Camera/CameraComponent.h" // ���������� ������
#include "Components/InputComponent.h" // ���������� �����
#include "GameFramework/SpringArmComponent.h" // ���������� �������� �� ������ (��� ������)
#include "Components/STUCharacterMovementComponent.h" // ���������� ��������������� ��������� ��� ������ character
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h" // ��� ������������ ����� Pawn

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer &ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{// ������� ����������� ������������� ������. ������� SetDefaultSubobjectClass ��� ������ ���������� �� ���������
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true; // ����� ����� ���� �������
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f); // �������� ������������ ������

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent"); // ���� ��������� ����� ���������� ������� ��� �� ����� SetupAttachment

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true); // ��������� ��������� ������ ��

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent"); // ���� ��������� ����� ���������� ������� ��� �� ����� SetupAttachment
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    check(HealthComponent); // �������� �� nullptr ����� ������ check - ���������� Assertion, �������� ������ � Developend � Debug ������
    check(HealthTextComponent);
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

    //const auto Health = HealthComponent->GetHealth(); // ������ ��� �� ����� ���������� ���������� HealthComponent �������� ���������� Health

    //TakeDamage(0.1f, FDamageEvent{}, Controller, this); // �������� ������� ���� ��� ���� TakeDamage (������� ����� �� ����� �������, DamageEvent ����������� �� ��������� (��� 2 ���� DamageType), ��������� �� ���������� ��������� �������� ������� �����, ��������� �� ������ ������� ������� �����)
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

	if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);

        PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput); // ��� �������� ������ �� ��������� �� ����� ������������ ������� AddControllerPitchInput(�������� ���� Pit
    // ch) ������� ���� � ������� Pawn. ����� �� ���������� ��������
        PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput); // ��� �������� ������ ������ Character ���������� ������� AddControllerYawInput

        PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump); // ������� ������ Jump. IE_Pressed - ��������� Controlle (����� ������� ������)

        PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning); // IE_Pressed ������ ������
        PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning); // IE_Released ������ ������
        
        PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASTUBaseCharacter::OnStartFire); // ������ �������� �� ������
        PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire); // ����� �������� �� ������

        PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent,&USTUWeaponComponent::NextWeapon); // ���� �� ����� ������

         PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload); // ���� �����������
    }
}

bool ASTUBaseCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero() && !IsMovingRight; // �������� ��� ������ ������ � ��� �������� ��������� �����, � ��� �������� ��������� �� ����� 0 + �������� �� ��������� � �������
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

void ASTUBaseCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f; // true ���� Amount > 0 - ������ ������� �� �������� �����
    if (Amount == 0.0f)
    {
         return;
    }
    AddMovementInput(GetActorForwardVector(), Amount); // ��� ����������� ������ ��������� ������������� �������� AddMovementInput(����������� �
                        // ������� ����� ���������� ������ ���������, scale) - ������ ������� ���������� � ������� Pawn
    if (IsRunning() && WeaponComponent->IsFiring()) // ���� �� ����� � �������� ��������
    {
         WeaponComponent->StopFire(); // ������������� ��������
    }
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;
    IsMovingRight = true; // �������� ������ ������� �� �������� � ���
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::OnStartRunning() // ����� ������� ������
{
    WantsToRun = true;
    IsMovingRight = false; // ���� �� ������ �� ��������� �������
    if (IsRunning()) // ���� �������� ��������
    {
         WeaponComponent->StopFire(); // ������������� ��������
    }
}

void ASTUBaseCharacter::OnStopRunning() // ����� ������� ������
{
    WantsToRun = false;
}

void ASTUBaseCharacter::OnDeath()
{
    // UE_LOG(LogBaseCharacter, Display, TEXT("Player %s is dead!"), *GetName()); // ��������� ��� ��� �������� �����

    // PlayAnimMontage(DeathAnimMontage); // ��� ������������ ������������� ������� ������� PlayAnimMontage(��������� �� ��������)

    GetCharacterMovement()->DisableMovement(); // ������������� �� �������� �������� ������� DisableMovement

    SetLifeSpan(LifeSpanOnDeath); // ����������� ������ ���������

    if (Controller) // �������� �� ���������� ���������
    {
        Controller->ChangeState(NAME_Spectating); // ���� �������� ���, �� �� ������ �� NAME_Spectating (��������� ������)
    }
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // ������ ������������� ������� ��� ������ ���������
    WeaponComponent->StopFire(); // ������������� �������� ��� ������ ���������
    // ��������� ������ ��������� ������ ����� �������� DeathAnimMontage
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ������� ������� �������� ������� ���������� SetCollisionEnabled()
    GetMesh()->SetSimulatePhysics(true); // ������� SetSimulatePhysics() - ���������� ���������� ��������� ����
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health))); // SetText ��������� ���������� ���� FText � ������������� ��������� �������� FromString � ����������� � FString
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult &Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z; // ����� ��� �������� ������������� � ������������, ��� ��� �������� �������� �������������������� ������ � ������� ������ ���� �����
    // UE_LOG(LogBaseCharacter, Display, TEXT("On landed: %f"), FallVelocityZ); // ����� �������� �������

    if (FallVelocityZ < LandedDamageVelocity.X) // �������� ���� �������� ������� ������ 900, ������� �� �������
    {
        return;
    }

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ); // ���������� ������. ������� GetMappedRangeValueClamped(������� �������, ����, �������� � ������� �����)
    // UE_LOG(LogBaseCharacter, Display, TEXT("FinalDamage: %f"), FinalDamage);
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
