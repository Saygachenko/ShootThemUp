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
	USTUNextLocationBTTaskNode(); // конструктор

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // функци€ вызываетс€ когда начинаетс€ выполнение нашего “аска(будем вычисл€ть рандомную точку вокруг персонажа и записывать точку в blackboard )

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Radius = 1000.0f; // переменна€ радиуса

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector AimLocationKey; // переменна€ хранит ключи blackboard(в него мы будем записывать нашу рандомную точку локации)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool SelfCenter = true; // отвечает за нахождение новой точки на навигационном ћеше относительно себ€, либо относительно другого актора

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (EditCindition = "!SelfCenter"))
	FBlackboardKeySelector CenterActorKey; // хранитс€ ключ дл€ объекта, относительно какого мы будет находить рандомную точку на навигационном ћеше

};
