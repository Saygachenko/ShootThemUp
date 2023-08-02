// Shoo Them Up Game


#include "UI/STUSpectatorWidget.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(GetOwningPlayer()); // ��������� �� ��������� ��������
	if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) // ���� ��������� �� ������� �� ���������� ��� ������ �������� �� �������
	{
		return false; // ����� ���������� false
	}

	CountDownTime = RespawnComponent->GetRespawnCountDown(); // ����� ���������� � CountDownTime ������� ������� �������� �� ��������
	return true; // � ���������� true
}
