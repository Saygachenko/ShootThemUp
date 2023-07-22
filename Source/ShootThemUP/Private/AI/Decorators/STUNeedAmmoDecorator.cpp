// Shoo Them Up Game


#include "AI/Decorators/STUNeedAmmoDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
	NodeName = "Need Ammo"; // имя ноды в дереве поведения
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner(); // получаем указатель на ИИ контроллер
	if (!Controller) // если указатель не получен
	{
		return false; // выход из фукнции
	}

	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn()); // получаем указатель на кол-во патронов у ИИ персонажа
	if (!WeaponComponent) // если компонент не найден
	{
		return false; // выход из функции
	}
	
	return WeaponComponent->NeedAmmo(WeaponType); // иначе даём патроны выбранному оружия в WeaponType
}
