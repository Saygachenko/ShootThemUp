// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Player/Public/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree; // поведение дерева
class UWidgetComponent; // декларация виджета

UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()

 public:
    ASTUAICharacter(const FObjectInitializer &ObjInit); // конструктор нашего ИИ

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset; // храним указатель на дерево поведения

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthWidgetComponent; // проперти для компонента здоровья

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float HealthVisibilityDistance = 1000.f; // расстояние при которомы не видим бар ХП

	virtual void BeginPlay() override;

	virtual void OnDeath() override;
	virtual void OnHealthChanged(float Health, float HealthDelta) override; // функция изменения процента здоровья

private:
	void UpdateHealthWidgetVisibility(); // функция логики видимости бара ХП на расстоянии
};
