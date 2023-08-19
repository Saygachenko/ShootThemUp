// Shoo Them Up Game


#include "UI/STUGameOverWidget.h"
#include "STUGameModeBase.h"
#include "Player/Public/STUPlayerState.h"
#include "UI/STUPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "STUUtils.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USTUGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetWorld()) // ���� ��������� �� ��� ����������
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()); // �������� ��������� �� �������
        if (GameMode) // ���� ��������� ����������
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged); // ������������� �� ��� ������� ��������� ����
        }
    }

    if (ResetLevelButton) // ���� ��������� �� ������ ����������
    {
        ResetLevelButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnResetLevel); // ������������� �� �������, ����� �� ������� �� ������
    }
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
        PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor()); // ������������� ���� ������� � �������

        PlayerStatBox->AddChild(PlayerStatRowWidget); // ��������� ������� � ���� ������ ����������
    }
}

void USTUGameOverWidget::OnResetLevel()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this); // �������� ��������� �� ��� ������ � ������� ������� GetCurrentLevelName()
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName)); // OpenLevel(this ��� getworld(), ��� ������) - ������ ��������� �������(�� ���� ����� ��� � ��� ������ �������, �� ������ ��������)
}
