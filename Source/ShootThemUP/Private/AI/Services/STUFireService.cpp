// Shoo Them Up Game


#include "AI/Services/STUFireService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

USTUFireService::USTUFireService()
{
	NodeName = "Fire"; // �������� ����� ����
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner(); // ��������� �� ���������� AI
	const auto Blackboard = OwnerComp.GetBlackboardComponent(); // �������� ��������� �� blackboard

	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName); // ��������� Blackboard �� ����� 0 � ��� �� �� ����� 0 ������ ������� ��������� � Blackboard

	if (Controller) // ���� ��������� �� AI ����
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn()); // �������� WeaponComponent �� Pawn ������� ������� Controller
		if (WeaponComponent) // ���� WeaponComponent ����������
		{
			HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire(); // HasAim ���� ���� ���� �� ��������� ������� ��������, ���� ���� ������������� ��������
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); // ����� ������� ������� TickNode
}
