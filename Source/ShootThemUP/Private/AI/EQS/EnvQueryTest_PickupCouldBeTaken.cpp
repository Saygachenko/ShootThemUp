// Shoo Them Up Game


#include "AI/EQS/EnvQueryTest_PickupCouldBeTaken.h"
#include "Pickups/STUBasePickup.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"

UEnvQueryTest_PickupCouldBeTaken::UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low; // переменная отвечает за сложность теста (сложность зависит от математических операций)
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass(); // переменная зависит от того с каким типом данным будет работать наш Тест
	SetWorkOnFloatValues(false); // в зависимость от параметра устанавливается фильт тайп. Наш тест чисто логический поэтому переменная должна быть Match - данное значение становится если передать значение false
}

void UEnvQueryTest_PickupCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{   // код фильтрации. Флаг WantsBeTakable инверсирует наше условие
	const auto DataOwner = QueryInstance.Owner.Get(); // получаем владельца экземпляра запроса как указатель
	BoolValue.BindData(DataOwner, QueryInstance.QueryID); // связь объекта BoolValue с данными владельца и идентификатором запроса
	const bool WantsBeTakable = BoolValue.GetValue(); // получаем значение BoolValue как логическую переменную WantsBeTakable

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It) // ItemIterator - проходит по всем сгенерированным сущностям
	{
		const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex()); // получаем указатель на актор
		const auto PickupActor = Cast<ASTUBasePickup>(ItemActor); // преобразовываем ItemActor к ASTUBasePickup
		if (!PickupActor) // если PickupActor не найден
		{
			continue; // то переходим к следующему элементу
		}

		const auto CouldBeTaken = PickupActor->CouldBeTaken(); // указатель на функцию CouldBeTaken() - взятия пикапа (true) если да
		It.SetScore(TestPurpose, FilterType, CouldBeTaken, WantsBeTakable); // SetScore(переменная калсса, фильтр тайп (у нас он match), можем поднять или нет, желаемый результат теста)
	}
}
