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
	USphereComponent *CollisionComponent; // ���� ������� �����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    float RespawnTime = 5.0f; // ����� �������� ������

	UPROPERTY()
    TArray<APawn *> OverlappingPawns; // ������ ���������� �� APawn

	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override; // �������������� ������� , ���������� ����� � ����� ���� ����������� ������ ��������� ������� ECR_Overlap (���� � ��������)
    virtual void NotifyActorEndOverlap(AActor *OtherActor) override; // �������������� �������, ���������� ����� � ����� ���� ����������� ������ ��������� ������� ECR_Ignore (����� �� ��������)

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool CouldBeTaken() const; // ������� ������ ������ ��� EQS

private:
    float RotationYaw = 0.0f; // ���� �� ������� �� ����� ������� ��� �����
	FTimerHandle RespawnTimerHandle; // ��������� ������ ��������, ������ ��������� ����� ����������� 1 ��� (��������� ������ �� �����, ������� ��� �� ����� ��������� ��� ��������� ������ �� �����)

    virtual bool GivePickupTo(APawn *PlayerPawn); // ������� ������� �� ����� �������������� � ������ �������� � ������ (����� ���������� true ���� ��� ������� ����� �����)

    void PickupWasTaken(); // ������� �������� �� �� ��� �� ����� �����
	void Respawn(); // ������� ����� ����������� �� ������ RespawnTime

	void GenerateRotationYaw(); // ������� �������� �� ���������� ������������ ��� ���� ��������

};
