// Shoo Them Up Game


#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePickupTo(APawn *PlayerPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn); // получаем указатель на ХП
    if (!HealthComponent) // проверяем если компонента не существует или наш персонаж погиб
    {
        return false; // выходим из функции
    }

    return HealthComponent->TryToAddHealth(HealthAmount); // если персонаж живой то вызываем функцию TryToAddHealth
}
