#pragma once

#include "STUCoreTypes.generated.h"

class ASTUBaseWeapon;
class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, ASTUBaseWeapon*); // делегат оповещает о том что у нас закончились патроны + указатель на базовое оружие
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature); // делегат который оповещает нашего персонажа о смерти
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float); // делегат для контроля изменения ХП у персонажа
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, ESTUMatchState); // делегат состояний игры

USTRUCT(BLueprintType)
struct FAmmoData // структура боезапасов
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets = 0; // кол-во патронов в магазине

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
    int32 Clips = 0; // кол-во магазинов

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite = true; // определяем конечен арсенал данного оружия или нет
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

USTRUCT(BlueprintType)
struct FGameData // структура хранит данные в которой находятся все настройки игры
{
    GENERATED_USTRUCT_BODY()
    // кол-во игроков
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "100"))
    int32 PlayersNum = 2;
    // кол-во раундом
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
    int32 RoundsNum = 4;
    // продолжительность раунда
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
    int32 RoundTime = 10;
    // дефолтный цвет команды
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor DefaultTeamColor = FLinearColor::Red;
    // массив цветов из которого мы будет выбирать цвет для команды взависимости от ИД команды
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<FLinearColor> TeamColors;
    // время респавна
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "20"))
    int32 RespawnTime = 5;
};

UENUM(BlueprintType)
enum class ESTUMatchState : uint8 // enum класс которые описывает все состояния игры
{
    WaitingToStart = 0, // ожидание начала
    InProgress, // когда идёт игра
    Pause, // пауза
    GameOver // игра окончена
};