// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"


class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUBaseWeapon();

    FOnClipEmptySignature OnClipEmpty;

	virtual void StartFire(); // функция огня
    virtual void StopFire(); // функция огня

    void ChangeClip(); // замена текущей обоймы на новую
    bool CanReload() const; // проверяет , может-ли данное оружие делать перезарядку

    bool FireInProgress = false; // продолжительность стрельбы
    bool IsFiring() const;       // функция отмены стрельбы

    FWeaponUIData GetUIData() const // геттер для структуры
    {
        return UIData;
    }

    FAmmoData GetAmmoData() const // храним текущий арсенал (кол-во патронов, магазинов с патронами)
    {
        return CurrentAmmo;
    }

    bool TryToAddAmmo(int32 ClipsAmount); // логика добавления магазинов и патронов(хранит кол-во магазинов)
    bool IsAmmoEmpty() const; // будет возвращать true когда наш арсенал полностью пустой

protected:
	// Called when the game starts or when spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent *WeaponMesh; // компонент скелетного меша

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket"; // сокет кости

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f; // максимальная дистанция выстрела

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false}; // будет отвечать за начальный арсенал оружия

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData; // для каждого оружия нам нужно проинициализировать даунную структуру

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem *MuzzleFX; // проперти ниагары вспышки дула
	
	virtual void BeginPlay() override;

	virtual void MakeShot(); // функция логики выстрела
    APlayerController* GetPlayerController() const; // функция на получения контроля порсонажа
    bool GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRoration) const; // функция на получение доступа к камере
    FVector GetMuzzleWorldLocation() const; // функция возвращает положение сокета оружия на скелете
    virtual bool GetTraceData(FVector &TraceStart, FVector &TraceEnd) const; // функция начала линии и конца (выстрел)
    void MakeHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd) const; // функция пересечения линии с различными объектами

	void DecreaseAmmo(); // будет вызываться каждый раз после выстрела
    bool IsClipEmpty() const; // будет возвращать true когда текущая обойма у нас пустая
    void LogAmmo(); // вывод информацию про арсенал в консоль

    bool IsAmmoFull() const; // функция для улучшения читаемости кода (если магазин полон)

    UNiagaraComponent *SpawnMuzzleFX(); // функция спавна ниагары

private:
    FAmmoData CurrentAmmo; // будет отвечать за текущий арсенал оружия

};
