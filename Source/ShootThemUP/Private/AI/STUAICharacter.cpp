// Shoo Them Up Game


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer &ObjInit) : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // пешка автоматически переходит во владение AI-контроллера при каждом ее создании
    AIControllerClass = ASTUAIController::StaticClass(); // установили ИИ базовый контроллер

    bUseControllerRotationYaw = false; // RotationYaw выкл
    if (GetCharacterMovement()) // если true
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true; // ControllerDesiredRotation = true
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f); // изменяем скорость поворота
    }
}
