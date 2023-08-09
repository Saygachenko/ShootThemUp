// Shoo Them Up Game


#include "UI/STUPauseWidget.h"
#include "Gameframework/GameModeBase.h"
#include "Components/Button.h"

void USTUPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ClearPauseButton) // если указатель на кнопку существует
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause); // подписываемся на делегат, когда мы кликаем на кнопку
	}
}

void USTUPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) // если указателя на мир или гейм моде не существует
	{
		return; // выход из функции
	}

	GetWorld()->GetAuthGameMode()->ClearPause(); // вызываем функцию ClearPause - функция обратная от функции SetPause
}
