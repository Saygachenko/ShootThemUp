// Shoo Them Up Game


#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); // ������ begin play

	const auto STUCharacter = Cast<ASTUAICharacter>(InPawn); // ���� ����� ������� ��������� ������� OnPossess � ������ ASTUAICharacter
	if (STUCharacter) // ���� true
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset); // ��������� ������ ��������� RunBehaviorTree(��������� �� ������ ���������)
	}
}
