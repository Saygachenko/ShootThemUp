// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_PickupCouldBeTaken.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API UEnvQueryTest_PickupCouldBeTaken : public UEnvQueryTest
{
	GENERATED_BODY()
	
public:
	UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer); // наш конструктор
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const; // функция отвечает за всю нашу логику взятия пикапа. Функция запускается когда и наш EQS

};
