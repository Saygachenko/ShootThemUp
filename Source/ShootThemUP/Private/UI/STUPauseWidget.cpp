// Shoo Them Up Game


#include "UI/STUPauseWidget.h"
#include "Gameframework/GameModeBase.h"
#include "Components/Button.h"

bool USTUPauseWidget::Initialize()
{
	const auto InitStatus = Super::Initialize(); // делаем указатель на базовую функцию

	if (ClearPauseButton) // если указатель на кнопку существует
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause); // подписываемся на делегат, когда мы кликаем на кнопку
	}

	return InitStatus; // возвращаем значение переменной InitStatus
}

void USTUPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) // если указателя на мир или гейм моде не существует
	{
		return; // выход из функции
	}

	GetWorld()->GetAuthGameMode()->ClearPause(); // вызываем функцию ClearPause - функция обратная от функции SetPause
}
