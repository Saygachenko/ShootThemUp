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

	void SetShotDirection(const FVector& Direction) // ������� ����������� ��������
	{
        ShotDirection = Direction;
	}

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USphereComponent *CollisionComponent; // ������� �����

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    UProjectileMovementComponent *MovementComponent; // ������ ��������� �������� ������ �� ���, ������������ �� ����������� � �� ����������

	UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent *WeaponFXComponent; // ��� ��������� �������

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Weapon")
    float DamageRadius = 200.0f; // ������ �����

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Weapon")
    float DamageAmount = 50.0f; // ���� � ������

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Weapon")
    bool DoFullDamage = false; // ������ ����

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Weapon")
    float LifeSeconds = 5.0f; // ����� ������� �������� ������

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    FVector ShotDirection; // ������ ����������� ��������

	UFUNCTION()
    void OnProjectileHit (UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
    AController *GetController() const;
};
