// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGameDataWidget.generated.h"

class ASTUGameModeBase; // декларация
class ASTUPlayerState; // декларация

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUGameDataWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetCurrentRoundNum() const; // номер текущего раунда

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetTotalRoundsNum() const; // общее кол-во раундов

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetRoundSecondRemaining() const; // кол-во сек до конца раунда

private:
	ASTUGameModeBase* GetSTUGameMode() const; // возвращает наш GameMode
	ASTUPlayerState* GetSTUPlayerState() const; // возвращает наш PlayerState
};
