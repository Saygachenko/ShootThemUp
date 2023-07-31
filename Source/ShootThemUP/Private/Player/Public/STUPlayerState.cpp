// Shoo Them Up Game


#include "Player/Public/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerState, All, All);

void ASTUPlayerState::LogInfo()
{
	UE_LOG(LogPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}
