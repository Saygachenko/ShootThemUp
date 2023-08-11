// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

	FName GetStartupLevelName() const // получаем имя основного уровня
	{
		return StartupLevelName;
	}

	FName GetMenuLevelName() const // получаем имя уровня меню
	{
		return MenuLevelName;
	}

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName StartupLevelName = NAME_None; // явно храним название уровня (если оно none, выводим инфу об ошибке)

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName MenuLevelName = NAME_None; // явно храним название уровня (если оно none, выводим инфу об ошибке)
	
};
