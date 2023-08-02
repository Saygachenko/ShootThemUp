// Shoo Them Up Game


#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STURespawnComponent.h"

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent); // для компонентна нужно вызвать функцию AIController - SetPerceptionComponent(ссылка на созданный компонент)

	bWantsPlayerState = true; // Чтоб PlayerState у ИИ спавнился автоматически

	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent"); // создаём компонент респавна
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
	const auto AimActor = GetFocusOnActor(); // переменной присваиваем результат функции GetFocusOnActor - фокусировка на ближайшего актора
	SetFocus(AimActor); // SetFocus() - функция выставляем вращение контроллера таким образом, чтобы FVector указывал на актор который мы передадим ей в параметры
}

AActor* ASTUAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) // если GetBlackboardComponent возвращает false
	{
		return nullptr; // вовзращаем так же 0 
	}
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName)); // возвращаем объект на который нам нужно сфокусироваться
}
