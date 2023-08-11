// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUMenuWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton; // добавляем указатель на кнопку старта игры

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton; // добавляем указатель на кнопку выхода из игры

	virtual void NativeOnInitialized() override; // функция вызывается если инициализация прошла успешно

private:
	UFUNCTION()
	void OnStartGame(); // логика кнопки старта игры

	UFUNCTION()
	void OnQuitGame(); // логика кнопки выхода из игры
};