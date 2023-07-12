// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUAmmoPickup.generated.h"

class ASTUBaseWeapon;

UCLASS()
class SHOOTTHEMUP_API ASTUAmmoPickup : public ASTUBasePickup
{
	GENERATED_BODY()
  
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
    int32 ClipsAmount = 10; // кол-во магазинов которое может подобрать персонаж

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    TSubclassOf<ASTUBaseWeapon> WeaponType; // определяем какому оружию мы должны дать магизины

private:
    virtual bool GivePickupTo(APawn *PlayerPawn) override;
};
