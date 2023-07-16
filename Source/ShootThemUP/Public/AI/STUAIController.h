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
	USTUAIPerceptionComponent* STUAIPerceptionComponent; // указатель на компонент USTUAIPerceptionComponent (компонент позволяет ИИ определять других акторов в мире - видеть, слышать врагов, реагировать на урон)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FName FocusOnKeyName = "EnemyActor"; // хранится имя ключа в котором содержится указатель на вражеского персонажа

	virtual void OnPossess(APawn* InPawn) override; // переопределяем OnPossess (когда мы получили контроль над павном) - может вызываться несколько раз во время игры, у контроля может менятся Pawn
	virtual void Tick(float DeltaTime) override; // вызов тик функции

private:
	AActor* GetFocusOnActor() const; // функция для удобства, возвращает указатель на актора на которого нам нужно сфокусироваться
};
