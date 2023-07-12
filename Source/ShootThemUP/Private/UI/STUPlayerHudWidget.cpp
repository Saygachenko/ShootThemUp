// Shoo Them Up Game


#include "UI/STUPlayerHudWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

bool USTUPlayerHudWidget::Initialize()
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn()); // ������ Component � HealthComponent
    if (HealthComponent) // �������� �� 0
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHudWidget::OnHealthChanged); // ������������� �� ������� ��
    }

    return Super::Initialize(); // ���������� ����� Initialize �� �� ���������� true false
}

void USTUPlayerHudWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f) // ���� �������� ��������� �������� ������ ���� (��� ������� ���� ������ ���������)
    {
        OnTakeDamage(); // �������� ����� OnTakeDamage()
    }
}

float USTUPlayerHudWidget::GetHeatlhPercent() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn()); // ������ Component � HealthComponent
    if (!HealthComponent) // �������� ��� HealthComponent ���� ��� ����
    {
        return 0.0f;
    }

    return HealthComponent->GetHealthPercent(); // ���� HealthComponent �� 0, �� ���������� �������� ��� ������� GetHealthPercent
}

bool USTUPlayerHudWidget::GetCurrentWeaponUIData(FWeaponUIData &UIData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn()); // ������ Component � WeaponComponent
    if (!WeaponComponent) // �������� ��� WeaponComponent ���� ��� ����
    {
        return false;
    }

    return WeaponComponent->GetCurrentWeaponUIData(UIData); // ���� WeaponComponent �� true, �� ���������� �������� ��� ������� GetWeaponUIData
}

bool USTUPlayerHudWidget::GetCurrentWeaponAmmoData(FAmmoData &AmmoData) const
{
     const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn()); // �������� ��������� �� ��� ��������� ������� �������
                                             // GetComponentByClass(��������� ������� �� ����� �����)
    if (!WeaponComponent) // �������� ��� WeaponComponent ���� ��� ����
    {
        return false;
    }

    return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData); // ���� WeaponComponent �� true, �� ���������� �������� ��� ������� GetWeaponAmmoData
}

bool USTUPlayerHudWidget::IsPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHudWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer(); // GetOwningPlayer() - ������� �������� ������� �������� ������
    return Controller && Controller->GetStateName() == NAME_Spectating;
}
