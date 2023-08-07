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
public:
	virtual bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget)) // мета информация ассоциирует кнопку котороая находится блюпринте с кнопкой которую мы будем использовать в С++
	UButton* ClearPauseButton; // храним кнопку паузы

private:
	UFUNCTION()
	void OnClearPause(); // логика кнопки паузы
	
};
