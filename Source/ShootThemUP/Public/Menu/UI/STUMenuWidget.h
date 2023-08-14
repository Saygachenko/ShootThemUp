// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class USTUGameInstance;
class USTULevelItemWidget;

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

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LevelItemsBox; // указатель на горизонтальную рамку

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LevelItemWidgetClass;// класс создаваемого виджета 

	virtual void NativeOnInitialized() override; // функция вызывается если инициализация прошла успешно

private:
	UPROPERTY()
	TArray<USTULevelItemWidget*> LevelItemWidgets; // в этом массиве храним указатель на созданные объекты виджетов

	UFUNCTION()
	void OnStartGame(); // логика кнопки старта игры

	UFUNCTION()
	void OnQuitGame(); // логика кнопки выхода из игры

	void InitLevelItems(); // основная логига создания кнопок клеток (наших уровней)
	void OnLevelSelected(const FLevelData& Data); // call back для нашего делегата структуры уровня(логика когда мы выбрали уровень)
	
	USTUGameInstance* GetSTUGameInstance() const; // указатель на наш GameInstance
};