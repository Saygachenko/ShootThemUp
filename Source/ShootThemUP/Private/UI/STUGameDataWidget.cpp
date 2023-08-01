// Shoo Them Up Game


#include "UI/STUGameDataWidget.h"
#include "STUGameModeBase.h"
#include "Player/Public/STUPlayerState.h"

int32 USTUGameDataWidget::GetKillsNum() const
{
	const auto PlayerState = GetSTUPlayerState(); // получаем указатель на PlayerState
	return PlayerState ? PlayerState->GetKillsNum() : 0; // если PlayerState существует то возвращаем функции кол-во убийств, иначе 0
}

int32 USTUGameDataWidget::GetCurrentRoundNum() const
{
	const auto GameMode = GetSTUGameMode(); // получаем указатель на GameMode
	return GameMode ? GameMode->GetCurrentRoundNum() : 0; // если GameMode существует то возвращаем номер текущего раунда, иначе 0
}

int32 USTUGameDataWidget::GetTotalRoundsNum() const
{
	const auto GameMode = GetSTUGameMode(); // получаем указатель на GameMode
	return GameMode ? GameMode->GetGameData().RoundsNum : 0; // если GameMode существует то возвращаем общее кол-во раундов, иначе 0
}

int32 USTUGameDataWidget::GetRoundSecondRemaining() const
{
	const auto GameMode = GetSTUGameMode(); // получаем указатель на GameMode
	return GameMode ? GameMode->GetRoundSecondsRemaining() : 0; // если GameMode существует то возвращаем время текущего раунда, иначе 0
}

ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const
{
	return GetWorld() ? Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr; // если мир существует, возвращаем указатель на наш GameMode, иначе nullptr
}

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const
{
	return GetOwningPlayer() ? Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState) : nullptr; // если контроллер владельца виджета существует, то вызываем указатель на PlayerState, иначе nullptr
}
