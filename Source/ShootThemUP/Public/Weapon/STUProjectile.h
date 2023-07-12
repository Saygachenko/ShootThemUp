// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class USTUWeaponFXComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUProjectile();

	void SetShotDirection(const FVector& Direction) // функция направления выстрела
	{
        ShotDirection = Direction;
	}

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USphereComponent *CollisionComponent; // колизия сферы

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    UProjectileMovementComponent *MovementComponent; // данный компонент изменяет актора на тик, взависемости от настроенных в нём параметров

	UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent *WeaponFXComponent; // наш компонент Ниагары

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Weapon")
    float DamageRadius = 200.0f; // радиус урона

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Weapon")
    float DamageAmount = 50.0f; // урон в центре

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Weapon")
    bool DoFullDamage = false; // полный урон

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Weapon")
    float LifeSeconds = 5.0f; // через сколько удалится снаряд

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    FVector ShotDirection; // хранит направление выстрела

	UFUNCTION()
    void OnProjectileHit (UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
    AController *GetController() const;
};
