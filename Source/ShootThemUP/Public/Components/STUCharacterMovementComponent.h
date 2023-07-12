// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "STUCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUCharacterMovementComponent : public UCharacterMovementComponent // �������������� ��������� CharacterMovementComponent
{
	GENERATED_BODY()
  public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.5", clampMax = "10.0")) // ��������� meta - ����������� ���������� �������� ��� ����������
        float RunModifier = 2.0f; // �������� ��� ���� ����� ������������� � 2 ����.

    virtual float GetMaxSpeed() const override; // �������������� ������������ �������� ���������
	
};
