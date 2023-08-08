// Shoo Them Up Game


#include "UI/STUGameOverWidget.h"
#include "STUGameModeBase.h"
#include "Player/Public/STUPlayerState.h"
#include "UI/STUPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "STUUtils.h"

bool USTUGameOverWidget::Initialize()
{
    if (GetWorld()) // если указатель на мир существует
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()); // получаем указатель на гейммод
        if (GameMode) // если указатель существует
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged); // подписываемся на наш делегат состояния игры
        }
    }

	return Super::Initialize();
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::GameOver) // если текущее состояние игры равно GameOver
    {
        UpdatePlayerStat(); // вызываем функцию статистики
    }
}

void USTUGameOverWidget::UpdatePlayerStat()
{
    if (!GetWorld() || !PlayerStatBox) // если мир или вертикальный бокс не существуют
    {
        return; // выход из функции
    }

    PlayerStatBox->ClearChildren(); // удалит в боксе все дочерние элементы

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) // проходимся по всем контроллерам в мире
    {
        const auto Controller = It->Get(); // получаем указатель на контроллер
        if (!Controller) // если указателя не существует
        {
            continue; // пропуск условия
        }

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState); // получаем указатель на PlayerState контроллера
        if (!PlayerState) // если указателя на PlayerState не существует
        {
            continue; // пропуск условия
        }

        const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass); // создаём для каждого контроллера виджет статистики
        if (!PlayerStatRowWidget) // если указателя на созданный виджет статистики не существует
        {
            continue; // пропуск условия
        }

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName())); // устанавливаем имя персонажа в статистику. Функция GetPlayerName возвращает стринг, меняем на текст
        PlayerStatRowWidget->SetKills(STUUtils::TextFromInt(PlayerState->GetKillsNum())); // устанавливаем кол-во убийств в статистику. Используем нашу функцию TextFromInt
        PlayerStatRowWidget->SetDeaths(STUUtils::TextFromInt(PlayerState->GetDeathsNum())); // устанавливаем кол-во смертей в статистику. Используем нашу функцию TextFromInt
        PlayerStatRowWidget->SetTeam(STUUtils::TextFromInt(PlayerState->GetTeamID())); // устанавливаем номер команды в статистику. Используем нашу функцию TextFromInt
        PlayerStatRowWidget->SetPlayerIndicatorVisibillity(Controller->IsPlayerController()); // устанавливаем выделение контроллера в true, в статистике.

        PlayerStatBox->AddChild(PlayerStatRowWidget); // добавляем строчку в нашу виджет статистику
    }
}
