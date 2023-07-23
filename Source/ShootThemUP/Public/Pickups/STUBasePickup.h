// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUBasePickup();

protected:
	// Called when the game starts or when spawned
	USphereComponent *CollisionComponent; // наша колизия сферы

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    float RespawnTime = 5.0f; // время респавна пикапа

	UPROPERTY()
    TArray<APawn *> OverlappingPawns; // массив указателей на APawn

	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override; // переопределяем функцию , вызывается когда с каким либо компонентом актора произошло событие ECR_Overlap (вход в коллизию)
    virtual void NotifyActorEndOverlap(AActor *OtherActor) override; // переопределяем функцию, вызывается когда с каким либо компонентом актора произошло событие ECR_Ignore (выход из коллизии)

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool CouldBeTaken() const; // функция взятия пикапа для EQS

private:
    float RotationYaw = 0.0f; // угол на который мы хотим вращать наш пикап
	FTimerHandle RespawnTimerHandle; // запускаем таймер респавна, таймер локальный будет срабатывать 1 раз (отключать таймер не будем, поэтому его не нужно создавать как переменну класса не нужно)

    virtual bool GivePickupTo(APawn *PlayerPawn); // функция которую мы будем переопределять в пикапе здоровья и оружия (будет возвращать true если нам удалось взять пикап)

    void PickupWasTaken(); // функция отвечает за то что мы взяли пикап
	void Respawn(); // функция будет срабатывать по вызову RespawnTime

	void GenerateRotationYaw(); // функция отвечает за вычисление необходимого нам угла поворота

};
