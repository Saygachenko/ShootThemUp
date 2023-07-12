// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
	
public:
    ASTURifleWeapon();

    virtual void StartFire() override; // функция огня
    virtual void StopFire() override;  // функция огня


protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f; // частота срабатывания таймера выстрела

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float BulletSpread = 1.5f; // разброс выстрела

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmout = 10.0f; // урон от попадания

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent *WeaponFXComponent; // наш компонент Ниагары

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem *TraceFX; // храним указатель на трейс эффект

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget"; // хранится название переменной, для конечно точки нашего эффекта

    virtual void BeginPlay() override;

    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector &TraceStart, FVector &TraceEnd) const override; // функция начала линии и конца (выстрел)

private:

    FTimerHandle ShotTimerHandle; // таймер для стрельбы из винтовки

    UPROPERTY()
    UNiagaraComponent *MuzzleFXComponent; // храним указатель на ниагар компонент

    void MakeDamage(const FHitResult &HitResult); // функция нанесения урона от линии

    void InitMuzzleFX(); // спавн нашей ниагары если указатель 0 (вспышка дула)
    void SetMuzzleFXVisibility(bool Visible); // функция видимости нашей ниагар системы

    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd); // функция отвечает за спавн эффекта трейса
};
