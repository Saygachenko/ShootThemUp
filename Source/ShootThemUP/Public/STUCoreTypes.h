#pragma once

#include "STUCoreTypes.generated.h"

class ASTUBaseWeapon;
class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, ASTUBaseWeapon*); // делегат оповещает о том что у нас закончились патроны + указатель на базовое оружие
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature); // делегат который оповещает нашего персонажа о смерти
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float); // делегат для контроля изменения ХП у персонажа

USTRUCT(BLueprintType)
struct FAmmoData // структура боезапасов
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets; // кол-во патронов в магазине

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
    int32 Clips; // кол-во магазинов

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite; // определяем конечен арсенал данного оружия или нет
};

USTRUCT(BlueprintType)
struct FWeaponData // в структуре храним оружие и анимацию перезарядки оружия
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage *ReloadAnimMontage;
};

USTRUCT(BlueprintType) 
struct FWeaponUIData // структура изображения прицела оружия
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D *MainIcon; // картинка оружия

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D *CrossHairIcon; // картинка прицела
    
};

USTRUCT(BlueprintType)
struct FDecalData // структура для настройки Decal
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UMaterialInterface *Material; // материал Decal

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FVector Size = FVector(10.0f); // размер (с одним параметром, устанавливает все остальные вектора по 10 тоже)

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float LifeTime = 5.0f; // время жизни (сколько он будет находится на сцене)

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float FadeOutTime = 0.7f; // время анимации (за данный промежуток времени будет растворяться)
};

USTRUCT(BlueprintType)
struct FImpactData // структура объеденяет Ниагару и Декаль
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem *NiagaraEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FDecalData DecalData;
};