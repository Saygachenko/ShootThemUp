// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPauseWidget.generated.h"

class UButton; // декларация класса кнопки

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget)) // мета информация ассоциирует кнопку котороая находится блюпринте с кнопкой которую мы будем использовать в С++
	UButton* ClearPauseButton; // храним кнопку паузы

	virtual void NativeOnInitialized() override; // функция вызывается если инициализация прошла успешно

private:
	UFUNCTION()
	void OnClearPause(); // логика кнопки паузы
	
};
