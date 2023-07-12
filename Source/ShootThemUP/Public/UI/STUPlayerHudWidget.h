// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHudWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHeatlhPercent() const; // текущее значение процентов ХП

	UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponUIData(FWeaponUIData &UIData) const; // возврашает UIData текущего оружия

	UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponAmmoData(FAmmoData &AmmoData) const; // возвращаем Ammo (арсенал) текущего оружия

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const; // функция возвращает true - когда персонаж живой, false - когда погиб

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerSpectating() const; // функция возвращает true - когда мы в режиме наблюдателя

    UFUNCTION(BlueprintImplementableEvent, Category = "UI") // спецификатор BlueprintImplementableEvent - позволяет создать евент на освное функции 
    void OnTakeDamage(); // делаем чтобы наш евент был видел в блюпринтах (не нужно создавать телов в cpp)

    virtual bool Initialize() override; // евент похож на BeginPlay, вызывается 1 раз при старте игры (тут мы будем подписываться на делегат OnHealthChanged)

private:
    void OnHealthChanged(float Health, float HealthDelta); // функция изменения здоровья
};
