// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUCoreTypes.h"
#include "STUPlayerController.generated.h"

class USTURespawnComponent;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASTUPlayerController();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USTURespawnComponent* RespawnComponent; // указатель на респавн компонент

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override; // переопределяем функцию OnPossess
	virtual void SetupInputComponent() override; // функция для бинд кнопок

private:
	void OnPauseGame(); // функция логики паузы
	void OnMatchStateChanged(ESTUMatchState State); // функция для вызова информации об изменениях состоянии игры
};
