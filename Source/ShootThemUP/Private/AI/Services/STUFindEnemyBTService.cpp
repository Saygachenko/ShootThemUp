// Shoo Them Up Game


#include "AI/Services/STUFindEnemyBTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"

USTUFindEnemyBTService::USTUFindEnemyBTService()
{
	NodeName = "Find Enemy"; // �������� ����� ����
}

void USTUFindEnemyBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent(); // �������� ��������� �� blackboard
	if (Blackboard) // ���� blackboard ����������
	{
		const auto Controller = OwnerComp.GetAIOwner(); // ��������� �� ���������� AI
		const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller); // ��������� �� PerceptionComponent - ��������� ���������� (���������� ���������� ���������� Actor)
		if (PerceptionComponent) // ���� PerceptionComponent ������
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy()); // ���������� �������� � Blackboard, SetValueAsObject(����, ��������� �� UObject)
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); // ����� ������� ������� TickNode
}
