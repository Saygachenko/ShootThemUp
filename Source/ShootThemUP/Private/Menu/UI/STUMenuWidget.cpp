// Shoo Them Up Game


#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton) // если кнопка существует
	{
		StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame); // подписываемся на делегат нажатия кнопки
	}
}

void USTUMenuWidget::OnStartGame()
{
	const FName StartupLevelName = "TestLevel"; // делаем указатель на наш уровень
	UGameplayStatics::OpenLevel(this, StartupLevelName); // загружаем наш уровень
}
