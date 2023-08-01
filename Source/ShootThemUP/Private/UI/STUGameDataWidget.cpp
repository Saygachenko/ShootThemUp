// Shoo Them Up Game


#include "UI/STUGameDataWidget.h"
#include "STUGameModeBase.h"
#include "Player/Public/STUPlayerState.h"

int32 USTUGameDataWidget::GetKillsNum() const
{
	const auto PlayerState = GetSTUPlayerState(); // �������� ��������� �� PlayerState
	return PlayerState ? PlayerState->GetKillsNum() : 0; // ���� PlayerState ���������� �� ���������� ������� ���-�� �������, ����� 0
}

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

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const
{
	return GetOwningPlayer() ? Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState) : nullptr; // ���� ���������� ��������� ������� ����������, �� �������� ��������� �� PlayerState, ����� nullptr
}
