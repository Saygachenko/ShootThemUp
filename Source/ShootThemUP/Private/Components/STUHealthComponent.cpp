// Shoo Them Up Game


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h" // библиотека тряски камеры
//#include "Dev/STUFireDamageType.h" // наш тип урона огонь
//#include "Dev/STUIceDamageType.h" // наш тип урона лёд
#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // в данном классе нам не понадобится тик


}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0); // чтобы не было деления на 0 и отрицательных значений

	SetHealth(MaxHealth); // каждый раз когда игра будет запускается, хп у персонажа будет 100

	AActor *ComponentOwner = GetOwner(); // GetOwner возвращает на владельца данного компонента
	if (ComponentOwner) // если есть владелец у данного компонента (не нулевой)
	{
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage); // подписались на делегат OnTakeAnyDamage
	}

	
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld()) // если урон меньше или равно 0, если мы уже мертвы, если не мир
	{
        return;
	}

	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle); // если мы начали получать урон, удаляем наш таймер автохила

	if (IsDead()) // ещё одна проверка, живы мы или нет
	{
		Killed(InstigatedBy); // функция убийцы
        OnDeath.Broadcast(); // вызов нашего делегата, таким образом мы оповестим всех клиентов которые подписаны на делегат о смерти
	}
    else if (AutoHeal) // если мы живы то запуститься автохил
	{
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}

	PlayCameraShake(); // вызываем тряску камеры при получении урона
    //UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);

	/*if (DamageType) для теста
	{
		if (DamageType->IsA<USTUFireDamageType>()) // используем функцию UObject IsA<принимает тип объекта>(возвращает true если данный объект относится к этому типу)
		{
            UE_LOG(LogHealthComponent, Display, TEXT("Fire Damage"));
		}
		else if (DamageType->IsA<USTUIceDamageType>())
		{
            UE_LOG(LogHealthComponent, Display, TEXT("Ice Damage"));
		}
	}*/
}

void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier); // учеличения нашего ХП

	if (IsHealthFull() && GetWorld()) // проверка на то что текущее ХП равно максимальному. Числа с плавающей точкой
                                    // проверяем через функцию IsNearlyEqual()
	{
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle); // если оно равно, удаляем наш таймер
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth); // Ограничение минимального и максимального хп. Функция Clamp(значени переменной, граница отрезка, граница отрезка)
    const auto HealthDelta = NextHealth - Health; // записываем разницу между след ХП и текущим

    Health = NextHealth; // Health присваиваем NextHealth
    OnHealthChanged.Broadcast(Health, HealthDelta); // оповещение всех клиентов что наше ХП изменилось HealthDelta - величина изменения здоровья
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsDead() || IsHealthFull()) // если персонаж мёртв, или полная шкала здоровья
	{
        return false;
	}
    SetHealth(Health + HealthAmount); // добавляем макмимальное число жизней к текущему
    return true;
}

bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth); // если текущее ХП равно максимальному то возвращаем true
}

void USTUHealthComponent::PlayCameraShake()
{
	if (IsDead()) // проверка что наш персонаж мёртв
	{
        return;
	}

	const auto Player = Cast<APawn>(GetOwner()); // указатель на павна (в нём есть указатель на котроллер)
	if (!Player) // если персонажем никто не управляет
	{
        return;
	}

	const auto Controller = Player->GetController<APlayerController>(); // получаем указатель на контроллер
	if (!Controller || !Controller->PlayerCameraManager) // если указателя котроля нету, или указатель PlayerCameraManager нулевой
	{
        return;
	}

	Controller->PlayerCameraManager->StartCameraShake(CameraShake); // вызываем функцию StartCameraShake

}

void USTUHealthComponent::Killed(AController* KillerController)
{
	if (!GetWorld()) // если мир не существует
	{
		return; // выход из функции
	}

	const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()); // указатель на гейм мод
	if (!GameMode) // если указатель на гейм мод не сущетвует
	{
		return; // выход из функцииы
	}

	const auto Player = Cast<APawn>(GetOwner()); // указатель на нашего Character
	const auto VictimController = Player ? Player->Controller : nullptr; // если наш персонаж существует, вызываем его контроллер иначе присваиваем nullptr

	GameMode->Killed(KillerController, VictimController); // вызываем функция килл
}
