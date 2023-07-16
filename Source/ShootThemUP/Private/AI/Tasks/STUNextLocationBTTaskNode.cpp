// Shoo Them Up Game


#include "AI/Tasks/STUNextLocationBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

USTUNextLocationBTTaskNode::USTUNextLocationBTTaskNode()
{
	NodeName = "Next Location"; // NodeName - её значение выводится в визуальной ноде в едиторе "название нашей ноды"
}

EBTNodeResult::Type USTUNextLocationBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner(); // указатель на контроллер AI
	const auto Blackboard = OwnerComp.GetBlackboardComponent(); // указатель на Blackboard
	if (!Controller || !Blackboard) // если Controller или Blackboard не true
	{
		return EBTNodeResult::Failed; // наш таск заканчивае работу со статусом Failed
	}

	const auto Pawn = Controller->GetPawn(); // указатель на Pawn
	if (!Pawn) // если не true
	{
		return EBTNodeResult::Failed; // наш таск заканчивае работу со статусом Failed
	}

	const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn); // указатель на UNavigationSystemV1 - класс описывает текущую навигационную систему (нужен для нахождения рандомной точки), чтобы получить указатель на навигационную систему, нужно вызвать статическую функцию GetCurrent(принимает UObject)
	if (!NavSys) // если не true
	{
		return EBTNodeResult::Failed; // наш таск заканчивае работу со статусом Failed
	}

	FNavLocation NavLocation; // переменная хранит найденную точку. Возвращает true или false
	auto Location = Pawn->GetActorLocation(); // указатель на координаты Павна
	if (!SelfCenter) // если SelfCenter = false
	{
		auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName)); // меняем центр на навигационном меше относительно имени ключа который мы указали в Blackboard
		if (!CenterActor) // если центр Актора не найден
		{
			return EBTNodeResult::Failed; // наш таск заканчивае работу со статусом Failed
		}
		Location = CenterActor->GetActorLocation(); // иначе передаём новую позицию данного Актора
	}

	const auto Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation); // функция которая вызывает рандомную точку GetRandomReachablePointInRadius(центр относительно которого мы ищем точку, радиус поиска, найденная точка) - под капотом алгоритм поиска пути
	if (!Found) // если не true
	{
		return EBTNodeResult::Failed; // наш таск заканчивае работу со статусом Failed
	}

	Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location); // если точка найдена, нужно записать её в Blackboard->SetValueAsVector(имя ключа, FVector)
	return EBTNodeResult::Succeeded; // возвращаем успешный статус Succeeded
}
