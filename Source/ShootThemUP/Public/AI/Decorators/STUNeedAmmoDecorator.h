// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUNeedAmmoDecorator.generated.h"

class ASTUBaseWeapon; // декларация базового оружия

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUNeedAmmoDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	USTUNeedAmmoDecorator(); // конструктор
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<ASTUBaseWeapon> WeaponType; // проперти для выбора оружия которому нужны будут патроны


	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override; // в данной функции вычисляем необходимые условия поднятия пикапа патронов ИИ

};
