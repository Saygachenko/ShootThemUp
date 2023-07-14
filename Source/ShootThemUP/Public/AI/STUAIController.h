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
	ASTUAIController(); // ��� �����������
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USTUAIPerceptionComponent* STUAIPerceptionComponent; // ��������� �� ��������� USTUAIPerceptionComponent (��������� ��������� �� ���������� ������ ������� � ���� - ������, ������� ������, ����������� �� ����)

	virtual void OnPossess(APawn* InPawn) override; // �������������� OnPossess (����� �� �������� �������� ��� ������) - ����� ���������� ��������� ��� �� ����� ����, � �������� ����� ������� Pawn
	virtual void Tick(float DeltaTime) override;
};
