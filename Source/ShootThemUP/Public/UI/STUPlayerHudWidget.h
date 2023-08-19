// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHudWidget.generated.h"

class UProgressBar;

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

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetKillsNum() const; // ���-�� ������� ����������

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthProgressBar; // �������� ������ ���� ��

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float PercentColorThreshold = 0.3f; // �������� �������� �� ���� ���� � ����������� �� ���-�� ��

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor GoodColor = FLinearColor::White; // �������� �������� �� ���� ������ ����

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor BadColor = FLinearColor::Red; // �������� �������� �� ���� ������ ����

    virtual void NativeOnInitialized() override; // ������� ���������� ���� ������������� ������ �������

private:
    void OnHealthChanged(float Health, float HealthDelta); // ������� ��������� ��������
    void OnNewPawn(APawn* NewPawn); // ������� ������ �����
    void UpdateHealthBar(); // ������� ������ ���������� ����� �� ����
};
