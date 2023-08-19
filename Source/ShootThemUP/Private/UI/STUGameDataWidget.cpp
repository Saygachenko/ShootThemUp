// Shoo Them Up Game


#include "UI/STUGameDataWidget.h"
#include "STUGameModeBase.h"

int32 USTUGameDataWidget::GetCurrentRoundNum() const
{
	const auto GameMode = GetSTUGameMode(); // �������� ��������� �� GameMode
	return GameMode ? GameMode->GetCurrentRoundNum() : 0; // ���� GameMode ���������� �� ���������� ����� �������� ������, ����� 0
}

int32 USTUGameDataWidget::GetTotalRoundsNum() const
{
	const auto GameMode = GetSTUGameMode(); // �������� ��������� �� GameMode
	return GameMode ? GameMode->GetGameData().RoundsNum : 0; // ���� GameMode ���������� �� ���������� ����� ���-�� �������, ����� 0
}

int32 USTUGameDataWidget::GetRoundSecondRemaining() const
{
	const auto GameMode = GetSTUGameMode(); // �������� ��������� �� GameMode
	return GameMode ? GameMode->GetRoundSecondsRemaining() : 0; // ���� GameMode ���������� �� ���������� ����� �������� ������, ����� 0
}

ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const
{
	return GetWorld() ? Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr; // ���� ��� ����������, ���������� ��������� �� ��� GameMode, ����� nullptr
}
