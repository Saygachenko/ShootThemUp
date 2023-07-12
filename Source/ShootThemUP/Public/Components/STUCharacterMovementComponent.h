// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "STUCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUCharacterMovementComponent : public UCharacterMovementComponent // переопределяем компонент CharacterMovementComponent
{
	GENERATED_BODY()
  public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.5", clampMax = "10.0")) // механизим meta - минимальные допустимые значения для переменной
        float RunModifier = 2.0f; // скорость при беге будел увиличиваться в 2 раза.

    virtual float GetMaxSpeed() const override; // переопределяем максимальную скорость персонажа
	
};
