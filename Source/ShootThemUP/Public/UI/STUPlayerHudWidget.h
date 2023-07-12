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
    float GetHeatlhPercent() const; // ������� �������� ��������� ��

	UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponUIData(FWeaponUIData &UIData) const; // ���������� UIData �������� ������

	UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponAmmoData(FAmmoData &AmmoData) const; // ���������� Ammo (�������) �������� ������

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const; // ������� ���������� true - ����� �������� �����, false - ����� �����

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerSpectating() const; // ������� ���������� true - ����� �� � ������ �����������

    UFUNCTION(BlueprintImplementableEvent, Category = "UI") // ������������ BlueprintImplementableEvent - ��������� ������� ����� �� ������ ������� 
    void OnTakeDamage(); // ������ ����� ��� ����� ��� ����� � ���������� (�� ����� ��������� ����� � cpp)

    virtual bool Initialize() override; // ����� ����� �� BeginPlay, ���������� 1 ��� ��� ������ ���� (��� �� ����� ������������� �� ������� OnHealthChanged)

private:
    void OnHealthChanged(float Health, float HealthDelta); // ������� ��������� ��������
};
