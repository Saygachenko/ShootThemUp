// Shoo Them Up Game


#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent); // ��� ����������� ����� ������� ������� AIController - SetPerceptionComponent(������ �� ��������� ���������)
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); // ������ begin play

	const auto STUCharacter = Cast<ASTUAICharacter>(InPawn); // ���� ����� ������� ��������� ������� OnPossess � ������ ASTUAICharacter
	if (STUCharacter) // ���� true
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset); // ��������� ������ ��������� RunBehaviorTree(��������� �� ������ ���������)
	}
}

void ASTUAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = STUAIPerceptionComponent->GetClosestEnemy(); // ���������� ����������� ��������� ������� GetClosestEnemy
	SetFocus(AimActor); // SetFocus() - ������� ���������� �������� ����������� ����� �������, ����� FVector �������� �� ����� ������� �� ��������� �� � ���������
}
