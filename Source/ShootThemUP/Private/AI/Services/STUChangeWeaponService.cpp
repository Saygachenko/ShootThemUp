// Shoo Them Up Game


#include "AI/Services/STUChangeWeaponService.h"
#include "Components/STUWeaponComponent.h"
#include "AIController.h"
#include "STUUtils.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
	NodeName = "Change Weapon"; // ��� ����� ����� (�������)
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner(); // ��������� �� ���������� ��
	if (Controller) // ���� ���������� ����������
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn()); // �������� WeaponComponent �����
		if (WeaponComponent && Probability > 0 && FMath::FRand() <= Probability) // ���� WeaponComponent ���������� � Probability ������ ����, � ��������� ����� ������ ��� ����� Probability (0.0f - 1.0f)
		{
			WeaponComponent->NextWeapon(); // ������ ������
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
