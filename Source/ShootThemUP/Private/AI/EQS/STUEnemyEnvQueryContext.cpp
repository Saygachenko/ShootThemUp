// Shoo Them Up Game


#include "AI/EQS/STUEnemyEnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void USTUEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get()); // указатель на актор который владеет данным EQS

	const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner); // указатель на blackboard component GetBlackboard(указатель на актор)
	if (!Blackboard) // если указателя нету
	{
		return; // выходим из функции
	}

	const auto ContextActor = Blackboard->GetValueAsObject(EnemyActorKeyName); // указатель на объект ключа
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(ContextActor)); // сюда передаётся указатель на актор GetValueAsObject возвращает указатель на UObject, поэтому сделали каст к Actor
}
