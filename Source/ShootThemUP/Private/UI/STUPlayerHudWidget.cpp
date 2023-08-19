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

    if (GetOwningPlayer()) // если контроллер существует
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHudWidget::OnNewPawn); // подписываемся на делегат GetOnNewPawnNotifier
        OnNewPawn(GetOwningPlayerPawn()); // вызываем нашу функцию нового павна
    }
}

void USTUPlayerHudWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f) // если величина изменения здоровья меньше нуля (был нанесен урон нашему персонажу)
    {
        OnTakeDamage(); // вызываем евент OnTakeDamage()
    }

    UpdateHealthBar(); // наша функция изменения цвета по условию
}

void USTUPlayerHudWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn); // кастим Component к HealthComponent
    if (HealthComponent && !HealthComponent->OnHealthChanged.IsBoundToObject(this)) // проверка что указатель на ХП есть и что бинда на делегат нету для нашего объекта
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHudWidget::OnHealthChanged); // подписываемся на делегат ХП
    }

    UpdateHealthBar(); // наша функция изменения цвета по условию
}

void USTUPlayerHudWidget::UpdateHealthBar()
{
    if (HealthProgressBar) // если указатель на прогресс бар ХП существует
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHeatlhPercent() > PercentColorThreshold ? GoodColor : BadColor); // устанавливаем цвет ХП бара от условия Текущее ХП большем чем PercentColorThreshold, ставим белый иначе красный
    }
}

float USTUPlayerHudWidget::GetHeatlhPercent() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn()); // кастим Component к HealthComponent
    if (!HealthComponent) // проверка что HealthComponent есть или нету
    {
        return 0.0f;
    }

    return HealthComponent->GetHealthPercent(); // если HealthComponent не 0, то возвращаем значение его функции GetHealthPercent
}

bool USTUPlayerHudWidget::GetCurrentWeaponUIData(FWeaponUIData &UIData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn()); // кастим Component к WeaponComponent
    if (!WeaponComponent) // проверка что WeaponComponent есть или нету
    {
        return false;
    }

    return WeaponComponent->GetCurrentWeaponUIData(UIData); // если WeaponComponent не true, то возвращаем значение его функции GetWeaponUIData
}

bool USTUPlayerHudWidget::GetCurrentWeaponAmmoData(FAmmoData &AmmoData) const
{
     const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn()); // получаем указатель на наш компонент функция акторов
                                             // GetComponentByClass(компонент который мы хотим найти)
    if (!WeaponComponent) // проверка что WeaponComponent есть или нету
    {
        return false;
    }

    return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData); // если WeaponComponent не true, то возвращаем значение его функции GetWeaponAmmoData
}

bool USTUPlayerHudWidget::IsPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHudWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer(); // GetOwningPlayer() - функция получает текущий контроль игрока
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

int32 USTUPlayerHudWidget::GetKillsNum() const
{
    const auto Controller = GetOwningPlayer(); // GetOwningPlayer() - функция получает текущий контроль игрока
    if (!Controller) // если указателя не существует
    {
        return 0; // возвращаем 0
    }

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState); // получаем указатель на PlayerState
    return PlayerState ? PlayerState->GetKillsNum() : 0; // если PlayerState существует то возвращаем функции кол-во убийств, иначе 0
}
