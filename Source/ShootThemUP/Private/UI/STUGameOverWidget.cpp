// Shoo Them Up Game


#include "UI/STUGameOverWidget.h"
#include "STUGameModeBase.h"
#include "Player/Public/STUPlayerState.h"
#include "UI/STUPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "STUUtils.h"

bool USTUGameOverWidget::Initialize()
{
    if (GetWorld()) // ���� ��������� �� ��� ����������
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()); // �������� ��������� �� �������
        if (GameMode) // ���� ��������� ����������
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged); // ������������� �� ��� ������� ��������� ����
        }
    }

	return Super::Initialize();
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::GameOver) // ���� ������� ��������� ���� ����� GameOver
    {
        UpdatePlayerStat(); // �������� ������� ����������
    }
}

void USTUGameOverWidget::UpdatePlayerStat()
{
    if (!GetWorld() || !PlayerStatBox) // ���� ��� ��� ������������ ���� �� ����������
    {
        return; // ����� �� �������
    }

    PlayerStatBox->ClearChildren(); // ������ � ����� ��� �������� ��������

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) // ���������� �� ���� ������������ � ����
    {
        const auto Controller = It->Get(); // �������� ��������� �� ����������
        if (!Controller) // ���� ��������� �� ����������
        {
            continue; // ������� �������
        }

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState); // �������� ��������� �� PlayerState �����������
        if (!PlayerState) // ���� ��������� �� PlayerState �� ����������
        {
            continue; // ������� �������
        }

        const auto PlayerStatRowWidget = CreateWidget<USTUPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass); // ������ ��� ������� ����������� ������ ����������
        if (!PlayerStatRowWidget) // ���� ��������� �� ��������� ������ ���������� �� ����������
        {
            continue; // ������� �������
        }

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName())); // ������������� ��� ��������� � ����������. ������� GetPlayerName ���������� ������, ������ �� �����
        PlayerStatRowWidget->SetKills(STUUtils::TextFromInt(PlayerState->GetKillsNum())); // ������������� ���-�� ������� � ����������. ���������� ���� ������� TextFromInt
        PlayerStatRowWidget->SetDeaths(STUUtils::TextFromInt(PlayerState->GetDeathsNum())); // ������������� ���-�� ������� � ����������. ���������� ���� ������� TextFromInt
        PlayerStatRowWidget->SetTeam(STUUtils::TextFromInt(PlayerState->GetTeamID())); // ������������� ����� ������� � ����������. ���������� ���� ������� TextFromInt
        PlayerStatRowWidget->SetPlayerIndicatorVisibillity(Controller->IsPlayerController()); // ������������� ��������� ����������� � true, � ����������.

        PlayerStatBox->AddChild(PlayerStatRowWidget); // ��������� ������� � ���� ������ ����������
    }
}
