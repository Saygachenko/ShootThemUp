// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "STUNextLocationBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUNextLocationBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	USTUNextLocationBTTaskNode(); // �����������

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // ������� ���������� ����� ���������� ���������� ������ �����(����� ��������� ��������� ����� ������ ��������� � ���������� ����� � blackboard )

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Radius = 1000.0f; // ���������� �������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector AimLocationKey; // ���������� ������ ����� blackboard(� ���� �� ����� ���������� ���� ��������� ����� �������)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool SelfCenter = true; // �������� �� ���������� ����� ����� �� ������������� ���� ������������ ����, ���� ������������ ������� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (EditCindition = "!SelfCenter"))
	FBlackboardKeySelector CenterActorKey; // �������� ���� ��� �������, ������������ ������ �� ����� �������� ��������� ����� �� ������������� ����

};
