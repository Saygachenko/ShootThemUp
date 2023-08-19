// Shoo Them Up Game


#include "UI/STUPlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTUPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	if (!PlayerNameTextBlock) // если указателя на имя не существует
	{
		return; // выход из функции
	}
	PlayerNameTextBlock->SetText(Text); // устанавливаем имя
}

void USTUPlayerStatRowWidget::SetKills(const FText& Text)
{
	if (!KillsTextBlock) // если указателя на кол-во убийств не существует
	{
		return; // выход из функции
	}
	KillsTextBlock->SetText(Text); // устанавливаем счёт
}

void USTUPlayerStatRowWidget::SetDeaths(const FText& Text)
{
	if (!DeathsTextBlock) // если указателя на кол-во смертей не существует
	{
		return; // выход из функции
	}
	DeathsTextBlock->SetText(Text); // устанавливаем счёт
}

void USTUPlayerStatRowWidget::SetTeam(const FText& Text)
{
	if (!TeamTextBlock) // если указателя на номер команды не существует
	{
		return; // выход из функции
	}
	TeamTextBlock->SetText(Text); // устанавливаем номер команды
}

void USTUPlayerStatRowWidget::SetPlayerIndicatorVisibillity(bool Visible)
{
	if (!PlayerIndicatorImage) // если указателя на индикатор выделения персонажа не существует
	{
		return; // выход из функции
	}
	PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden); // устанавливаем видимость индикатора от условаия Visible если он true
}

void USTUPlayerStatRowWidget::SetTeamColor(const FLinearColor& TeamColor)
{
	if (!TeamImage) // если указатель на картинку цвета команды не существует
	{
		return; // выход из функции
	}

	TeamImage->SetColorAndOpacity(TeamColor); // иначе вызываем команду цвета
}
