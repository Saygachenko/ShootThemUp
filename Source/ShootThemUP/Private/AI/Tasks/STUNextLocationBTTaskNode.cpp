// Shoo Them Up Game


#include "AI/Tasks/STUNextLocationBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

USTUNextLocationBTTaskNode::USTUNextLocationBTTaskNode()
{
	NodeName = "Next Location"; // NodeName - � �������� ��������� � ���������� ���� � ������� "�������� ����� ����"
}

EBTNodeResult::Type USTUNextLocationBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner(); // ��������� �� ���������� AI
	const auto Blackboard = OwnerComp.GetBlackboardComponent(); // ��������� �� Blackboard
	if (!Controller || !Blackboard) // ���� Controller ��� Blackboard �� true
	{
		return EBTNodeResult::Failed; // ��� ���� ���������� ������ �� �������� Failed
	}

	const auto Pawn = Controller->GetPawn(); // ��������� �� Pawn
	if (!Pawn) // ���� �� true
	{
		return EBTNodeResult::Failed; // ��� ���� ���������� ������ �� �������� Failed
	}

	const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn); // ��������� �� UNavigationSystemV1 - ����� ��������� ������� ������������� ������� (����� ��� ���������� ��������� �����), ����� �������� ��������� �� ������������� �������, ����� ������� ����������� ������� GetCurrent(��������� UObject)
	if (!NavSys) // ���� �� true
	{
		return EBTNodeResult::Failed; // ��� ���� ���������� ������ �� �������� Failed
	}

	FNavLocation NavLocation; // ���������� ������ ��������� �����. ���������� true ��� false
	const auto Found = NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), Radius, NavLocation); // ������� ������� �������� ��������� ����� GetRandomReachablePointInRadius(����� ������������ �������� �� ���� �����, ������ ������, ��������� �����) - ��� ������� �������� ������ ����
	if (!Found) // ���� �� true
	{
		return EBTNodeResult::Failed; // ��� ���� ���������� ������ �� �������� Failed
	}

	Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location); // ���� ����� �������, ����� �������� � � Blackboard->SetValueAsVector(��� �����, FVector)
	return EBTNodeResult::Succeeded; // ���������� �������� ������ Succeeded
}
