// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPlayerStatRowWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUPlayerStatRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPlayerName(const FText& Text); // функция установки значения
	void SetKills(const FText& Text); // функция установки значения
	void SetDeaths(const FText& Text); // функция установки значения
	void SetTeam(const FText& Text); // функция установки значения
	void SetPlayerIndicatorVisibillity(bool Visible); // функция видимости индикатора

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameTextBlock; // текстовый блок имени персонажа

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillsTextBlock; // текстовый блок киллов

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathsTextBlock; // текстовый блок смертей

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TeamTextBlock; // текстовый блок номер команды

	UPROPERTY(meta = (BindWidget))
	UImage* PlayerIndicatorImage; // картина выделения нашего персонажа
};
