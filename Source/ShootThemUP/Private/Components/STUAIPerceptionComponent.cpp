// Shoo Them Up Game


#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Sight.h" // библиотека зрени€ дл€ »»

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PercieveActors; // в данном массиве будут хранитс€ все акторы в поле видимости нашего AICharacter
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors); // функци€ котора€ получаетс€ всех акторов GetCurrentlyPerceivedActors(типа чувства, массив акторов)
	if (PercieveActors.Num() == 0) // если акторов нету
	{
		return nullptr; // выход из функции
	}

	const auto Controller = Cast<AAIController>(GetOwner()); // указатель на наш собственный павн, получим его с помощью контроллера
	if (!Controller) // если контрол€ нет
	{
		return nullptr; // выход из функции
	}

	const auto Pawn = Controller->GetPawn(); // указатель на павн
	if (!Pawn) // если павна нет
	{
		return nullptr; // выход из функции
	}

	float BestDistance = MAX_FLT; // переменна€ ближайшее рассто€ние = по умолчанию установилинекое большое число с помощью макроса MAX_FLT
	AActor* BestPawn = nullptr; // переменна€ котора€ хранит указатель на ближайшего актора = по умолчанию 0
	for (const auto PercieveActor : PercieveActors) // проходимс€ по массиву хранени€ акторов в зоне видимости
	{
		const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PercieveActor); // указатель на ’ѕ компонент

		const auto PercievePawn = Cast<APawn>(PercieveActor); // кастим воспри€тие зрени€ к павну
		const auto AreEnemies = PercievePawn && STUUtils::AreEnemies(Controller, PercievePawn->Controller); // если воспри€тие павна и функци€ проверки на врага true - это наш враг

		if (HealthComponent && !HealthComponent->IsDead() && AreEnemies) // если указатель получен и наш персонаж не мЄртв и найдет противник
		{
			const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size(); // считаем рассто€ние до актора получаем вектор между PercieveActor Pawn и находим длину вектора использую функцию FVectora Size
			if (CurrentDistance < BestDistance) // если дистанци€ до актора меньше чем ближайшее рассто€ние (первый раз так и будет)
			{
				BestDistance = CurrentDistance; // присваиваем переменной ближайшее рассто€ние = дистанци€ до актора
				BestPawn = PercieveActor; // и присваиваем переменной котора€ хранит указатель на ближайшего актора в зоне видимости = новых акторов (перезаписываем)
			}
		}
	}

	return BestPawn; // возвращаем указатель на ближайшего актора
}
