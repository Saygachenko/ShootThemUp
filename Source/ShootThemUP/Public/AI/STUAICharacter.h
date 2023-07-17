// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Player/Public/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree; // поведение дерева

UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()

 public:
    ASTUAICharacter(const FObjectInitializer &ObjInit); // конструктор нашего ИИ

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset; // храним указатель на дерево поведения

protected:
	virtual void OnDeath() override;
};
