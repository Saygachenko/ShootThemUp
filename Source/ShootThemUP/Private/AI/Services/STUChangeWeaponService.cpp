// Shoo Them Up Game


#include "AI/Services/STUChangeWeaponService.h"
#include "Components/STUWeaponComponent.h"
#include "AIController.h"
#include "STUUtils.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
	NodeName = "Change Weapon"; // им€ нашей нодый (сервиса)
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner(); // указатель на контроллер »»
	if (Controller) // если контроллер существует
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn()); // получаем WeaponComponent ѕавна
		if (WeaponComponent && Probability > 0 && FMath::FRand() <= Probability) // ≈сли WeaponComponent существует и Probability больше нул€, и случайное число меньше или равно Probability (0.0f - 1.0f)
		{
			WeaponComponent->NextWeapon(); // мен€ем оружие
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
