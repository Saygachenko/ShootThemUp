// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Player/Public/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent; // компонент для того, чтобы мы могли вращать камеру по орбите, вокруг персонажа
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
	
public:
	ASTUPlayerCharacter(const FObjectInitializer& ObjInit); // для того чтобы явно указать класс для charactermovementcomponent используем специальный конструктор с параметром
	// в который передаётся объект инициализации const FObjectInitializer& ObjInit

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	virtual void OnDeath() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool IsRunning() const override;

private:
	bool WantsToRun = false;
	bool IsMovingForward = false;
	bool IsMovingRight = false;

	void MoveForward(float Amount); // движение вперёд - назад
	void MoveRight(float Amount); // движение вправо - влево

	void OnStartRunning(); // функция начала ускорения
	void OnStopRunning(); // функция окончания ускорения
};
