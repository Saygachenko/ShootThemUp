// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
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

    virtual void BeginPlay() override;

private:
    void DrawCrossHair(); // функция отрисовки
	
};
