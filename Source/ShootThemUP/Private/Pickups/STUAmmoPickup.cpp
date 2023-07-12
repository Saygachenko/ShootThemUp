// Shoo Them Up Game


#include "Pickups/STUAmmoPickup.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePickupTo(APawn *PlayerPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn); // получаем указатель на ХП
    if (!HealthComponent || HealthComponent->IsDead()) // проверяем если компонента не существует или наш персонаж погиб
    {
        return false; // выходим из функции
    }
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn); // получаем указатель на оружия
    if (!WeaponComponent) // если компонент не существует
    {
        return false; // выходим из функции
    }
    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount); // если существует компонент вызываем функцию TryToAddAmmo(Тип оружия, кол-во магазинов)
}
