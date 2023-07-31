// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

class UCameraShakeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUHealthComponent();

	FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const // функция определяет умер наш персонаж или нет
    {
        return FMath::IsNearlyZero(Health); // функция проверки на 0 IsNearlyZero()
    }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const // переменная будет возвращать процент оставшихся жизней
	{
        return Health / MaxHealth;
	}

	float GetHealth() const // геттер возвращение нашего хп
	{
        return Health;
	}

	bool TryToAddHealth(float HealthAmount); // функция хранит здоровье которые мы дадим при взятии пикапа персонажем
    bool IsHealthFull() const; // вспомогательная функция фулл ХП

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (clampMin = "0.0", clampMax = "1000.0"))
    float MaxHealth = 100.0f; // максимальное значение хп

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true; // включение автохила

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal")) // meta = (EditCondition = "AutoHeal") - если наша перменная автохил включенная, то и будут включены дополнительные возможности
    float HealUpdateTime = 1.0f; // частота с которой будут добавляться ХП

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealDelay = 3.0f; // задержка перед вызовой таймера обновления

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 5.0f; // кол-во прибавления ХП

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> CameraShake; // тут можем выставить нашу тряску камеры

	virtual void BeginPlay() override;
	
private:
    float Health = 0.0f; // наше здоровье со значением по умолчанию
    FTimerHandle HealTimerHandle; // нащ таймер для автохп
	
	UFUNCTION() // OnTakeAnyDamage(актор которому был нанесён ущерб, урон, тип damage, контроллер который ответсвенный за ущерб, актор который нанёс ущерб)
	void OnTakeAnyDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType, class AController *InstigatedBy, AActor *DamageCauser);

	void HealUpdate(); // функция обновления хп
    void SetHealth(float NewHealth); // текущее ХП

	void PlayCameraShake(); // наша функция проигрывания тряски камеры

	void Killed(AController* KillerController); // вспомагательная функция киллера
};
