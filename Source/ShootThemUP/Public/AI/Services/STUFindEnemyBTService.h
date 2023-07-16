// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUFindEnemyBTService.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUFindEnemyBTService : public UBTService
{
	GENERATED_BODY()
	
public:
	USTUFindEnemyBTService(); // конструктор

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector EnemyActorKey; // будет хранитс€ ключ дл€ нашего вражеского персонажа

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override; // (собственный тик —ервиса) тут хранитс€ логика поиска противника находитс€ в этой функции
};
