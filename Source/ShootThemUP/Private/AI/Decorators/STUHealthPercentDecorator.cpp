// Shoo Them Up Game


#include "AI/Decorators/STUHealthPercentDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"

USTUHealthPercentDecorator::USTUHealthPercentDecorator()
{
	NodeName = "HealthPercent"; // имя ноды в дереве поведения
}

bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner(); // получаем указатель на ИИ контроллер
	if (!Controller) // если указатель не получен
	{
		return false; // выход из фукнции
	}
	
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(Controller->GetPawn()); // получаем указатель на кол-во жизней ИИ персонажа
	if (!HealthComponent || HealthComponent->IsDead()) // если компонент жизней не получен или наш персонаж мёртв
	{
		return false; // выход из функции
	}

	return HealthComponent->GetHealthPercent() <= HealthPercent; // проверка на то что текущее ХП ИИ меньше или равно минимального процента жизней для поднятия пикапа и пополняем ХП
}
