// Shoo Them Up Game


#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton) // если кнопка существует
	{
		StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame); // подписываемся на делегат нажатия кнопки
	}

	if (QuitGameButton) // если кнопка существует
	{
		QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame); // подписываемся на делегат нажатия кнопки
	}
}

void USTUMenuWidget::OnStartGame()
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

	if (STUGameInstance->GetStartupLevelName().IsNone()) // вызываем наш геттер имени уровня.IsNone если имени нету
	{
		UE_LOG(LogSTUMenuWidget, Error, TEXT("Level name is NONE")); // оповещение об ошибке
		return; // выход из функции
	}

	//const FName StartupLevelName = "TestLevel"; // делаем указатель на наш уровень
	UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevelName()); // загружаем наш уровень
}

void USTUMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true); // QuitGame(WorldContextObject, контроллер, enum, this) - функция позволяет завершить игру
}
