// Shoo Them Up Game


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/STUHealthBarWidget.h"
#include "Components/STUHealthComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer &ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent")) // ������� SetDefaultSubobjectClass ��� ������ ���������� �� ���������
{
    AutoPossessAI = EAutoPossessAI::Disabled; // ����� ������������� ��������� �� �������� AI-����������� ��� ������ �� ��������
    AIControllerClass = ASTUAIController::StaticClass(); // ���������� �� ������� ����������

    bUseControllerRotationYaw = false; // RotationYaw ����
    if (GetCharacterMovement()) // ���� true
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true; // ControllerDesiredRotation = true
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f); // �������� �������� ��������
    }

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent"); // ������ ������ ���������
    HealthWidgetComponent->SetupAttachment(GetRootComponent()); // ������������ � ��������� ����������
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // ����� ��� ��� ����� ����� ��� �����
    HealthWidgetComponent->SetDrawAtDesiredSize(true); // ������������� true ����� �� ������ ��� ��� � ����
}

void ASTUAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateHealthWidgetVisibility(); // ������ ��� ��������� ���� �������
}

void ASTUAICharacter::BeginPlay()
{
   Super::BeginPlay();
   check(HealthWidgetComponent); // �������� ������ ������ ���������� ��������
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath(); // �������� ������� ������� OnDeath()

    const auto STUController = Cast<AAIController>(Controller); // ��������� �� ����������� ��
    if (STUController && STUController->BrainComponent) // ���� ���� ��������� �� �� ���������� � �������� ���������
    {
        STUController->BrainComponent->Cleanup(); // �������� ������� ��������� ������ ��������� ����� �� ����� ������ ������
    }
}

void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta)
{
    Super::OnHealthChanged(Health, HealthDelta); // �������� ������� ������� ��

    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject()); // ��������� �� ��� ������ ��� ��������
    if (!HealthBarWidget) // ���� ��������� �� �����������
    {
        return; // ����� �� �������
    }

    HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent()); // ������������� ������� �� ������ ����
}

void ASTUAICharacter::UpdateHealthWidgetVisibility()
{
    if (!GetWorld() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()) // ���� ��������� �� ��� ��� ��������� �� ���������� ������ ��� ��������� �� ������ �� ����������
    {
        return; // ����� �� �������
    }

    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation(); // ��������� �� �������������� ����������� (������). ���� ���������� GetPawnm() - �� ��� ����� ���������� ������� �� ���������� ������� (������ �� ���� � �� ��� ���), ����� GetPawnOrSpectator() �������� ��������
    const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation()); // �������� ��������� �� ��������� ����� ������� � ��
    HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true); // ���� ��������� ������ ������ ��� �������� � HealthVisibilityDistance �� ��� ����� ��� ��
}
