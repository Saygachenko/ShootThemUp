// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUCoreTypes.h"
#include "STUGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FString TestString = "Hello Game!"; // поле для теста

	FLevelData GetStartupLevel() const // получаем имя основного уровня
	{
		return StartupLevel;
	}

	void SetStartupLevel(const FLevelData& Data) // сеттер установки уровня
	{
		StartupLevel = Data;
	}

	TArray<FLevelData> GetLevelsData() const // геттер уровня из массива
	{
		return LevelsData;
	}

	FName GetMenuLevelName() const // получаем имя уровня меню
	{
		return MenuLevelName;
	}

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName MenuLevelName = NAME_None; // явно храним название уровня (если оно none, выводим инфу об ошибке)

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TArray<FLevelData> LevelsData; // массив уровней

private:
	FLevelData StartupLevel; // начальный уровень
	
};
