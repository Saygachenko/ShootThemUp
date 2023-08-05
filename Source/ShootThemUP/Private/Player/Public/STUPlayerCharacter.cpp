// Shoo Them Up Game


#include "Player/Public/STUPlayerCharacter.h"
#include "Camera/CameraComponent.h" // ���������� ������
#include "Components/InputComponent.h" // ���������� �����
#include "GameFramework/SpringArmComponent.h" // ���������� �������� �� ������ (��� ������)
#include "Components/STUWeaponComponent.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{// ������� ����������� ������������� ������. ������� SetDefaultSubobjectClass ��� ������ ���������� �� ���������
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true; // ����� ����� ���� �������
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f); // �������� ������������ ������

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);

        PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::AddControllerPitchInput); // ��� �������� ������ �� ��������� �� ����� ������������ ������� AddControllerPitchInput(�������� ���� Pit
        // ch) ������� ���� � ������� Pawn. ����� �� ���������� ��������
        PlayerInputComponent->BindAxis("TurnAround", this, &ASTUPlayerCharacter::AddControllerYawInput); // ��� �������� ������ ������ Character ���������� ������� AddControllerYawInput

        PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUPlayerCharacter::Jump); // ������� ������ Jump. IE_Pressed - ��������� Controlle (����� ������� ������)

        PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::OnStartRunning); // IE_Pressed ������ ������
        PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::OnStopRunning); // IE_Released ������ ������

        PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASTUPlayerCharacter::OnStartFire); // ������ �������� �� ������
        PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire); // ����� �������� �� ������

        PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon); // ���� �� ����� ������

        PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload); // ���� �����������
    }
}

void ASTUPlayerCharacter::MoveForward(float Amount)
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

void ASTUPlayerCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;
    IsMovingRight = true; // �������� ������ ������� �� �������� � ���
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUPlayerCharacter::OnStartRunning() // ����� ������� ������
{
    WantsToRun = true;
    IsMovingRight = false; // ���� �� ������ �� ��������� �������
    if (IsRunning()) // ���� �������� ��������
    {
        WeaponComponent->StopFire(); // ������������� ��������
    }
}

void ASTUPlayerCharacter::OnStopRunning() // ����� ������� ������
{
    WantsToRun = false;
}

bool ASTUPlayerCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero() && !IsMovingRight; // �������� ��� ������ ������ � ��� �������� ��������� �����, � ��� �������� ��������� �� ����� 0 + �������� �� ��������� � �������
}

void ASTUPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    if (Controller) // �������� �� ���������� ���������
    {
        Controller->ChangeState(NAME_Spectating); // ���� �������� ���, �� �� ������ �� NAME_Spectating (��������� ������)
    }
}