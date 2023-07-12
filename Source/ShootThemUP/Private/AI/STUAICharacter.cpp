// Shoo Them Up Game


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer &ObjInit) : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // пешка автоматически переходит во владение AI-контроллера при каждом ее создании
    AIControllerClass = ASTUAIController::StaticClass(); // установили ИИ базовый контроллер
}
