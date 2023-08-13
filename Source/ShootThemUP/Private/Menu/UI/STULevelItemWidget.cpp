// Shoo Them Up Game


#include "Menu/UI/STULevelItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTULevelItemWidget::SetLevelData(const FLevelData& Data)
{
	LevelData = Data; // устанавливаем уровень

	if (LevelNameTextBlock) // если текст в виджете есть
	{
		LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName)); // устанавливаем имя уровня в меню
	}

	if (LevelImage) // если картинка в виджете естьы
	{
		LevelImage->SetBrushFromTexture(Data.LevelThumb); // устанавливаем картинку уровня в меню
	} 
}

void USTULevelItemWidget::SetSelected(bool IsSelected)
{
	if (FrameImage) // если рамка в виджете существует
	{
		FrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden); // ставим её видимой, иначе невидимой
	}
}

void USTULevelItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (LevelSelectButton) // если кнопка существует
	{
		LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked); // подписываемся на делегат нажатия кнопки
	}
}

void USTULevelItemWidget::OnLevelItemClicked()
{
	OnLevelSelected.Broadcast(LevelData); // оповещаем всех подписывающих о нашем нажатии на кнопку
}
