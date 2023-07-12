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

	void StartFire(); // ������� ������ ��������
    void StopFire(); // ������� ����� ��������
    void NextWeapon(); // ��������� ������
    void Reload(); // ������� �����������

	bool IsFiring() const; // ������� ������ ��������

	bool GetCurrentWeaponUIData(FWeaponUIData &UIData) const; // ���������� UIData �������� ������

	bool GetCurrentWeaponAmmoData(FAmmoData &AmmoData) const; // ���������� AmmoData ��������

	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount); // ������� ������ ��� ������ � ���-�� ���������

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData; // ������ ������ � �������� �����������

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket"; // ����� ������ ��� ����� ������ �� ������� � ����������

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket"; // �������� �� ��� ������ � �������� �� ����������� ������ �� �����

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage *EquipAnimMontage; // ��������� �� ����� ������

	// Called when the game starts
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; // ���������� ������ ��� ������ ���������� EndPlay - ���������� � ������� ���������� ��� ������ EndPlay ������������� ������

private:

	UPROPERTY()
	ASTUBaseWeapon *CurrentWeapon = nullptr; // ���������� ������

	UPROPERTY()
    TArray<ASTUBaseWeapon *> Weapons; // �������� ���� ������

	UPROPERTY()
    UAnimMontage *CurrentReloadAnimMontage = nullptr; // ������ ��������� �� �������� �����������

	int32 CurrentWeaponIndex = 0; // �������� ������ ������
    bool EquipAnimInProgress = false; // ����� ����� ����������� �������� ����� ������, ������ ���������� ����� true
    bool ReloadAnimInProgress = false; // ����� ����� ����������� �������� �����������, ������ ���������� ����� true

	void SpawnWeapons(); // ���� ������� ������ ������
	void AttachWeaponToSocket(ASTUBaseWeapon *Weapon, USceneComponent *SceneComponent, const FName &SocketName); // ����������� ������ � ������
    void EquipWeapon(int32 WeaponIndex); // ���������� ���������, ������ � �����
	
	void PlayAnimMontag(UAnimMontage* Animation); // ������� ����������� ��� Anim Montage

	void InitAnimations(); // � ���� ������� �� ����� ��� Notify � ���������� �� ����
    void OnEnquipFinished(USkeletalMeshComponent* MeshComp); // ������ ��� Notify - �������� ����� ������
    void OnReloadFinished(USkeletalMeshComponent *MeshComp); // ������ ��� Notify - �������� �����������

	bool CanFire() const; // ������� ����� ���������� true ����� �� ������ ��������
    bool CanEquip() const; // ������� ����� ���������� true ����� �� ����� ������� ������
    bool CanReload() const; // ������� ����� ���������� true ����� �� ����� ������������ ������

    void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon); // �������� ����� � ����� ���������
    void ChangeClip(); // (������� ��� ��������) �������� ���� � ��� ������������ ������� ������������� ���� �������� ��� ������� OnEmptyClip

};
