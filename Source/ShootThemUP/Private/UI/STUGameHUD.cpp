// Shoo Them Up Game


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h" // класс базового виджета
#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();

    //DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHudWidgetClass)); // добавляем в наш контейнер состояние игры "InProgress" и его виджет
    GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass)); // добавляем в наш контейнер состояние игры "Pause" и его виджет

    for (auto GameWidgetPair : GameWidgets) // пробегаемся по нашему контейнеру
    {
        const auto GameWidget = GameWidgetPair.Value; // получаем наш ключ из контейнера
        if (!GameWidget) // если виджета нету
        {
            continue; // пропускаем условие
        }

        GameWidget->AddToViewport(); // добавляем виджет на экран
        GameWidget->SetVisibility(ESlateVisibility::Hidden); // делаем виджет невидимым
    }

    if (GetWorld()) // если указатель на мир существует
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()); // получаем указатель на гейммод
        if (GameMode) // если указатель существует
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged); // подписываемся на наш делегат состояния игры
        }
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

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State)
{
    if (CurrentWidget) // проверяем что текущий виджет существует
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden); // делаем его невидимым
    }
    
    if (GameWidgets.Contains(State)) // проверяем что в нашем контейнере есть ключ с таким состоянием
    {
        CurrentWidget = GameWidgets[State]; // устанавливаем текущее состояние в наш виджет
    }

    if (CurrentWidget) // если виджет существует
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible); // делаем его видимым
    }

    UE_LOG(LogSTUGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State)); // выводим в логи информацию об изменениях состоинии игры GetValueAsString - преобразует значение enum в строку
}
