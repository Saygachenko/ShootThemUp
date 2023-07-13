// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STUAIController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void OnPossess(APawn* InPawn) override; // переопределяем OnPossess (когда мы получили контроль над павном) - может вызываться несколько раз во время у контроля может менятся Pawn
};
