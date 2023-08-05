// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USTUHealthComponent;
//class UTextRenderComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjInit); // для того чтобы явно указать класс для charactermovementcomponent используем специальный конструктор с параметром
	// в который передаётся объект инициализации const FObjectInitializer& ObjInit
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUHealthComponent *HealthComponent; // компонент для ХП

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	//UTextRenderComponent* HealthTextComponent; // компонент для текста над головой

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUWeaponComponent* WeaponComponent; // компонент для логики нашего оружия

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage *DeathAnimMontage; // наша анимация смерти

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanOnDeath = 5.0f; // продолжительности жизни после смерти

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f); // отвечает за высоту падения FVector2D(минимальное значение, максимальное) - предельная скорость с которой будет наноситься урон

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f); // отвечает за нанесение урона с падения

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	FName MaterialColorName = "Paint Color"; // отвечает за имя параметра у материала

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnDeath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

	void OnStartFire(); // старт стрельбы

	void SetPlayerColor(const FLinearColor& Color); // функция будет задавать цвет материалу персонажа

private:
    void OnHealthChanged(float Health, float HealthDelta);

	UFUNCTION() 
    void OnGroundLanded(const FHitResult& Hit); // наша функция падения с высоты FHitResul - содержит информацию о контакте двух объектов

};
