// Shoo Them Up Game


#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

// Sets default values for this component's properties
USTURespawnComponent::USTURespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USTURespawnComponent::Respawn(int32 RespawnTime)
{
	if (!GetWorld()) // если указатель на мир не существует
	{
		return; // выход из функции
	}

	RespawnCountDown = RespawnTime; // Присваиваем оставшемуся времени = время до респавна
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.0f, true); // запускаем таймер
}

void USTURespawnComponent::RespawnTimerUpdate()
{
	if (--RespawnCountDown == 0) // отнимает время на -1 и если оно равняется 0
	{
		if (!GetWorld()) // если мир не существует
		{
			return; // выход из функции
		}
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle); // удаляем таймер

		const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()); // получаем указатель на GameMode
		if (!GameMode) // если указателя не существует
		{
			return; // выход из функции
		}

		GameMode->RespawnRequest(Cast<AController>(GetOwner())); // вызываем функцию запрос респавна и передаём указатель на контроллер владельца компонента
	}
}

bool USTURespawnComponent::IsRespawnInProgress() const
{
	return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle); // проверка что мир существует и таймер RespawnTimerHandle активный
}

