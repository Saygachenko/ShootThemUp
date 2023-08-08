// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	void SetTeamID(int32 ID)
	{
		TeamID = ID;
	}
	int32 GetTeamID() const
	{
		return TeamID;
	}

	void SetTeamColor(const FLinearColor& Color)
	{
		TeamColor = Color;
	}
	FLinearColor GetTeamColor() const
	{
		return TeamColor;
	}

	void AddKill() // функция добавления киллов
	{
		++KillsNum;
	}
	int32 GetKillsNum() // геттер кол-во убийств
	{
		return KillsNum;
	}
	void AddDeath() // функция добавления смертей
	{
		++DeathsNum;
	}
	int32 GetDeathsNum() // геттер кол-во смертей
	{
		return DeathsNum;
	}

	void LogInfo(); // функция для вывода статистики в консоль (логи)

private:
	int32 TeamID; // ид команды
	FLinearColor TeamColor; // цвет команды

	int32 KillsNum = 0; // кол-во убийств
	int32 DeathsNum = 0; // кол-во смертей
};
