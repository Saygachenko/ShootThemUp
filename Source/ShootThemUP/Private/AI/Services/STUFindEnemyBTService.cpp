// Shoo Them Up Game


#include "AI/Services/STUFindEnemyBTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"

USTUFindEnemyBTService::USTUFindEnemyBTService()
{
	NodeName = "Find Enemy"; // название нашей ноды
}

void USTUFindEnemyBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent(); // получаем указатель на blackboard
	if (Blackboard) // если blackboard существует
	{
		const auto Controller = OwnerComp.GetAIOwner(); // указатель на контроллер AI
		const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller); // указатель на PerceptionComponent - компонент восприятия (зрительное нахождение ближайшего Actor)
		if (PerceptionComponent) // если PerceptionComponent найдет
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy()); // записываем значение в Blackboard, SetValueAsObject(ключ, указатель на UObject)
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); // вызов базовой функции TickNode
}
