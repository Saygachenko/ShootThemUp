// Shoo Them Up Game


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h" // класс базового виджета

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();

    //DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHudWidgetClass); // CreateWidget<тип виджета>(указатель на объект владельца, класс виджета который хотим создать) создаём виджет
    if (PlayerHudWidgetClass) // если виджет создался
    {
        PlayerHUDWidget->AddToViewport(); // вызываем функцию виджета AddToViewport(порядок отрисовки виджета)
    }
}

void ASTUGameHUD::DrawCrossHair()
{
    // TInterval(точку центра экрана) - для вычисления центра экрана
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f); // для получения размера экрана понадобиться дополнительный объект Canvas

    const float HalfLineSize = 10.0f; // размер половины линии
    const float LineThickness = 2.0f; // толщина линии
    const FLinearColor LineColor = FLinearColor::Green; // цвет прицела (линий)

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness); // DrawLine(начальная точка на экране X, Y, конечная точка на экране X, Y, цвет линии, толщина линии) функция худа, позволяет рисовать линии на экране
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
