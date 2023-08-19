// Shoo Them Up Game


#include "UI/STUPlayerHudWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"
#include "Components/ProgressBar.h"
#include "Player/Public/STUPlayerState.h"

void USTUPlayerHudWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer()) // ���� ���������� ����������
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHudWidget::OnNewPawn); // ������������� �� ������� GetOnNewPawnNotifier
        OnNewPawn(GetOwningPlayerPawn()); // �������� ���� ������� ������ �����
    }
}

void USTUPlayerHudWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f) // ���� �������� ��������� �������� ������ ���� (��� ������� ���� ������ ���������)
    {
        OnTakeDamage(); // �������� ����� OnTakeDamage()
    }

    UpdateHealthBar(); // ���� ������� ��������� ����� �� �������
}

void USTUPlayerHudWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn); // ������ Component � HealthComponent
    if (HealthComponent && !HealthComponent->OnHealthChanged.IsBoundToObject(this)) // �������� ��� ��������� �� �� ���� � ��� ����� �� ������� ���� ��� ������ �������
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHudWidget::OnHealthChanged); // ������������� �� ������� ��
    }

    UpdateHealthBar(); // ���� ������� ��������� ����� �� �������
}

void USTUPlayerHudWidget::UpdateHealthBar()
{
    if (HealthProgressBar) // ���� ��������� �� �������� ��� �� ����������
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHeatlhPercent() > PercentColorThreshold ? GoodColor : BadColor); // ������������� ���� �� ���� �� ������� ������� �� ������� ��� PercentColorThreshold, ������ ����� ����� �������
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

int32 USTUPlayerHudWidget::GetKillsNum() const
{
    const auto Controller = GetOwningPlayer(); // GetOwningPlayer() - ������� �������� ������� �������� ������
    if (!Controller) // ���� ��������� �� ����������
    {
        return 0; // ���������� 0
    }

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState); // �������� ��������� �� PlayerState
    return PlayerState ? PlayerState->GetKillsNum() : 0; // ���� PlayerState ���������� �� ���������� ������� ���-�� �������, ����� 0
}
