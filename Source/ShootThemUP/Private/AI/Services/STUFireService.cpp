// Shoo Them Up Game


#include "AI/Services/STUFireService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

USTUFireService::USTUFireService()
{
	NodeName = "Fire"; // название нашей ноды
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner(); // указатель на контроллер AI
	const auto Blackboard = OwnerComp.GetBlackboardComponent(); // получаем указатель на blackboard

	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName); // компонент Blackboard не равен 0 и так же не равен 0 объект который находится в Blackboard

	if (Controller) // если указатель на AI есть
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn()); // получаем WeaponComponent от Pawn которым владеет Controller
		if (WeaponComponent) // если WeaponComponent существует
		{
			HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire(); // HasAim если цель есть то запускаем функцию стрельбы, если нету останавливаем стрельбу
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); // вызов базовой функции TickNode
}
