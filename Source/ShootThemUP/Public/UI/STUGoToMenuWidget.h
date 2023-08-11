// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGoToMenuWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUGoToMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* GoToMenuButton; // добавляем указатель на кнопку

	virtual void NativeOnInitialized() override; // функция вызывается если инициализация прошла успешно

private:
	UFUNCTION()
	void OnGoToMenu(); // логика кнопки меню игры
};
