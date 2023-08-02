// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTURespawnComponent();

	void Respawn(int32 RespawnTime); // функция респавна

	int32 GetRespawnCountDown() const // геттер оставшиеся время респавна
	{
		return RespawnCountDown;
	}
	bool IsRespawnInProgress() const; // true когда таймер респавна запущен

private:
	FTimerHandle RespawnTimerHandle; // наш таймер респавна
	int32 RespawnCountDown = 0; // оставшиеся время респавна

	void RespawnTimerUpdate(); // функция апдейта таймера
		
};
