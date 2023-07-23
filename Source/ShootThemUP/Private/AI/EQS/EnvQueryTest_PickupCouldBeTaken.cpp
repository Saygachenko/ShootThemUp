// Shoo Them Up Game


#include "AI/EQS/EnvQueryTest_PickupCouldBeTaken.h"
#include "Pickups/STUBasePickup.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"

UEnvQueryTest_PickupCouldBeTaken::UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low; // ���������� �������� �� ��������� ����� (��������� ������� �� �������������� ��������)
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass(); // ���������� ������� �� ���� � ����� ����� ������ ����� �������� ��� ����
	SetWorkOnFloatValues(false); // � ����������� �� ��������� ��������������� ����� ����. ��� ���� ����� ���������� ������� ���������� ������ ���� Match - ������ �������� ���������� ���� �������� �������� false
}

void UEnvQueryTest_PickupCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{   // ��� ����������. ���� WantsBeTakable ����������� ���� �������
	const auto DataOwner = QueryInstance.Owner.Get(); // �������� ��������� ���������� ������� ��� ���������
	BoolValue.BindData(DataOwner, QueryInstance.QueryID); // ����� ������� BoolValue � ������� ��������� � ��������������� �������
	const bool WantsBeTakable = BoolValue.GetValue(); // �������� �������� BoolValue ��� ���������� ���������� WantsBeTakable

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It) // ItemIterator - �������� �� ���� ��������������� ���������
	{
		const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex()); // �������� ��������� �� �����
		const auto PickupActor = Cast<ASTUBasePickup>(ItemActor); // ��������������� ItemActor � ASTUBasePickup
		if (!PickupActor) // ���� PickupActor �� ������
		{
			continue; // �� ��������� � ���������� ��������
		}

		const auto CouldBeTaken = PickupActor->CouldBeTaken(); // ��������� �� ������� CouldBeTaken() - ������ ������ (true) ���� ��
		It.SetScore(TestPurpose, FilterType, CouldBeTaken, WantsBeTakable); // SetScore(���������� ������, ������ ���� (� ��� �� match), ����� ������� ��� ���, �������� ��������� �����)
	}
}
