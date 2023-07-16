// Shoo Them Up Game


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer &ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent")) // ������� SetDefaultSubobjectClass ��� ������ ���������� �� ���������
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // ����� ������������� ��������� �� �������� AI-����������� ��� ������ �� ��������
    AIControllerClass = ASTUAIController::StaticClass(); // ���������� �� ������� ����������

    bUseControllerRotationYaw = false; // RotationYaw ����
    if (GetCharacterMovement()) // ���� true
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true; // ControllerDesiredRotation = true
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f); // �������� �������� ��������
    }
}
