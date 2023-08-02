// Shoo Them Up Game


#include "UI/STUSpectatorWidget.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(GetOwningPlayer()); // указатель на компонент респавна
	if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) // если указатель на респавн не существует или таймер респавна не запущен
	{
		return false; // тогда возвращаем false
	}

	CountDownTime = RespawnComponent->GetRespawnCountDown(); // иначе записываем в CountDownTime сколько времени осталось до респавна
	return true; // и возвращаем true
}
