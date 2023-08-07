// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUCoreTypes.h"
#include "STUGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
	GENERATED_BODY()
 public:
    virtual void DrawHUD() override; // функция отвечает за отрисовку и вызывается каждый кадр, можем рисовать линии, прямоугольники, выводить текстуры, текст и т.к.д

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHudWidgetClass; // проперти в котором хранится класс PlayerHudWidgetClass

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PauseWidgetClass; // проперти паузы

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<ESTUMatchState, UUserWidget*> GameWidgets; // контейнер в котором мы храним состояние игры и его виджет
    
    UPROPERTY()
    UUserWidget* CurrentWidget = nullptr; // храним указатель на текущий виджет

    void DrawCrossHair(); // функция отрисовки
	
    void OnMatchStateChanged(ESTUMatchState State); // функция для вызова информации об изменениях состоянии игры
};
