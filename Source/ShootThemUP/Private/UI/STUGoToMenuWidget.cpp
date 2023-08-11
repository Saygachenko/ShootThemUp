// Shoo Them Up Game


#include "UI/STUGoToMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGoToMenuWidget, All, All);

void USTUGoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GoToMenuButton) // если кнопка существует
	{
		GoToMenuButton->OnClicked.AddDynamic(this, &USTUGoToMenuWidget::OnGoToMenu); // подписываемся на делегат нажатия кнопки
	}
}

void USTUGoToMenuWidget::OnGoToMenu()
{
	if (!GetWorld()) // если мир не существует
	{
		return; // выход из функции
	}

	const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>(); // указатель на GameInstance
	if (!STUGameInstance) // если STUGameInstance не сущетсвует
	{
		return; // выход из функции
	}

	if (STUGameInstance->GetMenuLevelName().IsNone()) // вызываем наш геттер имени уровня.IsNone если имени нету
	{
		UE_LOG(LogSTUGoToMenuWidget, Error, TEXT("Menu level name is NONE")); // оповещение об ошибке
		return; // выход из функции
	}

	UGameplayStatics::OpenLevel(this, STUGameInstance->GetMenuLevelName()); // загружаем наш уровень
}
