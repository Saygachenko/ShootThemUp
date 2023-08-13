// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STULevelItemWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTULevelItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FLevelSelectedSignature OnLevelSelected; // храним наш делегат

	void SetLevelData(const FLevelData& Data); // установка уровня виджету

	FLevelData GetLevelData() const // геттер структуры
	{
		return LevelData;
	}

	void SetSelected(bool IsSelected); // логика видимости рамки FrameImage

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* LevelSelectButton; // добавляем указатель на кнопку

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelNameTextBlock; // добавляем указатель имя уровня

	UPROPERTY(meta = (BindWidget))
	UImage* LevelImage; // указатель на картинку уровня

	UPROPERTY(meta = (BindWidget))
	UImage* FrameImage; // указатель на картинку рамки

	virtual void NativeOnInitialized() override; // функция вызывается если инициализация прошла успешно

private:
	FLevelData LevelData; // храним нашу структуру

	UFUNCTION()
	void OnLevelItemClicked(); // логика кнопки выбора уровня
};
