// Shoo Them Up Game


#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); // вместо begin play

	const auto STUCharacter = Cast<ASTUAICharacter>(InPawn); // каст павна который передаётся функции OnPossess к классу ASTUAICharacter
	if (STUCharacter) // если true
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset); // запускаем дерево поведения RunBehaviorTree(указатель на дерево поведения)
	}
}
