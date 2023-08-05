// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Player/Public/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent; // компонент для того, чтобы мы могли вращать камеру по орбите, вокруг персонажа
class USphereComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CameraCollisionComponent; // указатель на компонент сферы

	virtual void OnDeath() override;
	virtual void BeginPlay() override;

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

	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); // когда одна коллизия начинает пересикать другую
	
	UFUNCTION()
	void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex); // когда у коллизий нет точки пересечений

	void CheckCameraOverlap(); // функция проверяет пересикает-ли сферическая коллизия капсулу или нет
};
