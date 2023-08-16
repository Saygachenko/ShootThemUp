// Shoo Them Up Game


#include "UI/STUHealthBarWidget.h"
#include "Components/ProgressBar.h"

void USTUHealthBarWidget::SetHealthPercent(float Percent)
{
	if (!HealthProgressBar) // если указатель на наш прогресс бар не существует
	{
		return; // выход из функции
	}

	const auto HealthBarVisibility = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent)) ? ESlateVisibility::Hidden : ESlateVisibility::Visible; // если наш процент ХП больше заданого в проперти или 0 делаем бар невидимым, иначе видимым
	HealthProgressBar->SetVisibility(HealthBarVisibility); // стави наш прогрес бар видимым в зависимости от условия выше

	const auto HealthBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor; // если процент ХП больше заданного в проперти то ставит белый цвет хп, иначе красный
	HealthProgressBar->SetFillColorAndOpacity(HealthBarColor); // меняем цвет бара ХП в зависимости от условия выше
	
	HealthProgressBar->SetPercent(Percent); // установка текущего ХП в бар
}
