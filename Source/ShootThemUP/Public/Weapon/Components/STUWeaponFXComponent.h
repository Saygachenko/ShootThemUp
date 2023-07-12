// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponFXComponent.generated.h"

class UNiagaraSystem;
class UPhysicalMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUWeaponFXComponent();

	void PlayImpactFX(const FHitResult &Hit); // спавн эффекта в FHitResult - находится вся необходимая инфа для корректного спавна эффекта (положение в мире, нормаль и т.к.д)

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FImpactData DefaultImpactData; // дефолтный эффект содержит ниагару и декаль

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TMap<UPhysicalMaterial *, FImpactData> ImpactDataMap; // тут мы можем выставить физик материалы и ниагара эффекты и декаль

};
