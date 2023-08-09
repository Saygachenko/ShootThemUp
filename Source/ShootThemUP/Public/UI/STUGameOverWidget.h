// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerStatBox; // храним указатель на виртикал бокс

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerStatRowWidgetClass; // указатель на класс виджет статистики

	UPROPERTY(meta = (BindWidget))
	UButton* ResetLevelButton; // указатель на нашу кнопку перезагрузки карты

	virtual void NativeOnInitialized() override; // функция вызывается если инициализация прошла успешно

private:
	void OnMatchStateChanged(ESTUMatchState State); // функция для вызова информации об изменениях состоянии игры
	void UpdatePlayerStat(); // в функции будет создаваться динамически таблица статистики

	UFUNCTION()
	void OnResetLevel(); // функция логики кнопки перезагрузки уровня
	
};
