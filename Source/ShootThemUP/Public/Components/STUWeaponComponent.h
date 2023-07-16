// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUWeaponComponent();

	virtual void StartFire(); // функция начала стрельбы
    void StopFire(); // функция конца стрельбы
	virtual void NextWeapon(); // следуещее оружие
    void Reload(); // функция перезарядки

	bool IsFiring() const; // функция отмены стрельбы

	bool GetCurrentWeaponUIData(FWeaponUIData &UIData) const; // возврашает UIData текущего оружия

	bool GetCurrentWeaponAmmoData(FAmmoData &AmmoData) const; // возвращаем AmmoData арсенала

	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount); // функция хранит тип оружия и кол-во магазинов

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData; // храним оружие и анимацию перезарядки

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket"; // чтобы менять наш сокет оружия на скелете в блюпринтах

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket"; // отвечает за имя сокета к которому мы прикрепляем оружие на спине

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage *EquipAnimMontage; // указатель на смену оружия

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon = nullptr; // количество оружия

	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons; // хранится наше оружие

	int32 CurrentWeaponIndex = 0; // хранение индекс оружия

	// Called when the game starts
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // уничтожаем актора при вызове компонента EndPlay - вызывается у каждого компонента при вызове EndPlay родительского актора

	bool CanFire() const; // функция будет возвращать true когда мы сможем стрелять
	bool CanEquip() const; // функция будет возвращать true когда мы можем сменить оружие

	void EquipWeapon(int32 WeaponIndex); // экипировка персонажа, оружие в руках

private:

	UPROPERTY()
    UAnimMontage *CurrentReloadAnimMontage = nullptr; // храним указатель на анимацию перезарядки

    bool EquipAnimInProgress = false; // когда будет происходить анимация смены оружия, данная переменная будет true
    bool ReloadAnimInProgress = false; // когда будет происходить анимация перезарядки, данная переменная будет true

	void SpawnWeapons(); // наша функция спавна оружия
	void AttachWeaponToSocket(ASTUBaseWeapon *Weapon, USceneComponent *SceneComponent, const FName &SocketName); // прикрепляем оружие к сокету
	
	void PlayAnimMontag(UAnimMontage* Animation); // функция проигрывает наш Anim Montage

	void InitAnimations(); // в этой функции мы найдём наш Notify и подпишимся на него
    void OnEnquipFinished(USkeletalMeshComponent* MeshComp); // биндим наш Notify - анимация смены оружия
    void OnReloadFinished(USkeletalMeshComponent *MeshComp); // биндим наш Notify - анимация перезарядки

    bool CanReload() const; // функция будет возвращать true когда мы можем перезарядить оружие

    void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon); // обратная связь с нашим делегатом
    void ChangeClip(); // (функция для удобства) вызываем если у нас пользователь захотел перезарядится либо вызвался наш делегат OnEmptyClip

};
