// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Player/Public/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent; // ��������� ��� ����, ����� �� ����� ������� ������ �� ������, ������ ���������
/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
	
public:
	ASTUPlayerCharacter(const FObjectInitializer& ObjInit); // ��� ���� ����� ���� ������� ����� ��� charactermovementcomponent ���������� ����������� ����������� � ����������
	// � ������� ��������� ������ ������������� const FObjectInitializer& ObjInit

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

	void MoveForward(float Amount); // �������� ����� - �����
	void MoveRight(float Amount); // �������� ������ - �����

	void OnStartRunning(); // ������� ������ ���������
	void OnStopRunning(); // ������� ��������� ���������
};
