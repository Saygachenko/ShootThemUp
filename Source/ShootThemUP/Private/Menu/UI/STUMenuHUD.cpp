// Shoo Them Up Game


#include "Menu/UI/STUMenuHUD.h"
#include "Blueprint/UserWidget.h"

void ASTUMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	const auto MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass); // создаём виджет
	if (MenuWidget) // если виджет создался
	{
		MenuWidget->AddToViewport(); // добавляем его в наш Viewport
	}
}
