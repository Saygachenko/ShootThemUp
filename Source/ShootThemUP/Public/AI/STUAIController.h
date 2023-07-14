// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STUAIController.generated.h"

class USTUAIPerceptionComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASTUAIController(); // наш конструктор
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USTUAIPerceptionComponent* STUAIPerceptionComponent; // указатель на компонент USTUAIPerceptionComponent (компонент позвол€ет »» определ€ть других акторов в мире - видеть, слышать врагов, реагировать на урон)

	virtual void OnPossess(APawn* InPawn) override; // переопредел€ем OnPossess (когда мы получили контроль над павном) - может вызыватьс€ несколько раз во врем€ игры, у контрол€ может мен€тс€ Pawn
	virtual void Tick(float DeltaTime) override;
};
