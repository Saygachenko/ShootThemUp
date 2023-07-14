// Shoo Them Up Game


#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent); // для компонентна нужно вызвать функцию AIController - SetPerceptionComponent(ссылка на созданный компонент)
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); // вместо begin play

	const auto STUCharacter = Cast<ASTUAICharacter>(InPawn); // каст павна который передаётся функции OnPossess к классу ASTUAICharacter
	if (STUCharacter) // если true
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset); // запускаем дерево поведения RunBehaviorTree(указатель на дерево поведения)
	}
}

void ASTUAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = STUAIPerceptionComponent->GetClosestEnemy(); // переменной присваиваем результат функции GetClosestEnemy
	SetFocus(AimActor); // SetFocus() - функция выставляем вращение контроллера таким образом, чтобы FVector указывал на актор который мы передадим ей в параметры
}
