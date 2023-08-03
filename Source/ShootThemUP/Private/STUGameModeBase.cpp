// Shoo Them Up Game

#include "STUGameModeBase.h"
#include "Player/Public/STUBaseCharacter.h"
#include "Player/Public/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"
#include "Player/Public/STUPlayerState.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10; // ���������� ���������� ����������� ����� �������� �� ��������� ������

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass(); // Pawn - ������ ��� ��� Character ����������� �� ����
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass(); // ��������� PlayerState �����������
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots(); // ����� �����
    CreateTeamsInfo(); // ������� ��� ������������� ������� �� ��������

    CurrentRound = 1; // ������� �����
    StartRound(); // ����� ������
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>()) // ��������� ��������-�� ������ ��������� �� ������������
    {
        return AIPawnClass; // ���������� ������ ���������
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController); // ����� ���������� ������������ �������
}

void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld()) // ���� ��������� �� ��� �� ����������
    {
        return; // ����� �� �������
    }

    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i) // ���� ��� �� 0 �� GameData.PlayersNum -1 ������ ��� ������ ����� ���������� ��� ��������
    {
        FActorSpawnParameters SpawnInfo; // FActorSpawnParameters � ��� ���������� ��� ������ �������������� �������� ��� ������ ���� �������
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn ������������ ����� ������� ����� �������������� ��������. AlwaysSpawn - ����� ������

        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo); // ������� ������ SpawnActor<AAIController>(����� ����������� ������� ��������� � ��������, ��������� ������ ��������);
        RestartPlayer(STUAIController); // RestartPlayer(������� ����� �����������) - ������ �����
    }
}

void ASTUGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime; // ��������� ���������� ������� �������� �� ����� ������
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true); // ��������� ����������� ������, ���������� ������ �������
}

void ASTUGameModeBase::GameTimerUpdate()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i / Round: %i / %i"), RoundCountDown, CurrentRound, GameData.RoundsNum); // ��� ������� ������, ������� ����� � ���� �������

    if (--RoundCountDown == 0) // ��������� ���-�� ������� �� ����� �� 1 � ���� ��� ����� 0
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle); // ������� ��� ������
        if (CurrentRound + 1 <= GameData.RoundsNum) // ������� ����� +1 � ���� �� ������ ��� ����� ���� ����� �������
        {
            ++CurrentRound; // ������� ����� + 1
            ResetPlayers(); // ������� �������
            StartRound(); // ����� ������ ������
        }
        else
        {
            GameOver(); // ������� ��������� ����
        }
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    if (!GetWorld()) // ���� ���� �� ����������
    {
        return; // ����� �� �������
    }

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) // ���������� � ���� ����������� � ���� ����� �������� ����� �������� GetControllerIterator
    {
        ResetOnePlayer(It->Get()); // �������� ��� ������� ����������� �� ����� ResetOnePlayer. ��� ��������� ������ ��������� �������� ������� Get()
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn()) // ���� ���������� � ���� ������� �� ������� ����������
    {
        Controller->GetPawn()->Reset(); // �������� ������� Reset() - �������
    }
    RestartPlayer(Controller); // ������� ���������
    SetPlayerColor(Controller); // ��������� ����� �������
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld()) // ���� ���� �� ����������
    {
        return; // ����� �� �������
    }

    int32 TeamID = 1; // ���������� ������ �������
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) // ���������� � ���� ����������� � ���� ����� �������� ����� �������� GetControllerIterator
    {
        const auto Controller = It->Get(); // ����� ��������� �� ����������
        if (!Controller) // ���� ��������� �� ���������� ��
        {
            continue; // ������� ��������
        }

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState); // ��������� �� PlayerState
        if (!PlayerState) // ���� ��������� �� PlayerState ������� ��
        {
            continue; // ������� �������
        }

        PlayerState->SetTeaimID(TeamID); // ������������� ��� ��
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID)); // ������������� ���� �������

        SetPlayerColor(Controller); // ��������� ����� �������

        TeamID = TeamID == 1 ? 2 : 1; // ���� ��� ID ����� 1 ������� �� ������ �� 2 � ��������
    }
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num()) // ���� ��� �� ������ ��� ���-�� ������(��� �� ���������� 1 , � ������� ���������� � 0)
    {
        return GameData.TeamColors[TeamID - 1]; // ���������� ���� � ������� ��� ��
    }
    UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id: %i, set to default: %s"), TeamID, *GameData.DefaultTeamColor.ToString()); // ���� ������ ���������� ����� � ������ 
    return GameData.DefaultTeamColor; // ���������� ��������� ������� ���� �������
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller) // ���� ����������� �� ����������
    {
        return; // ����� �� �������
    }

    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn()); // ��������� �� character
    if (!Character) // ���� ��������� �� ��������� �� ����������
    {
        return; // ����� �� �������
    }

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState); // ��������� �� PlayerState
    if (!PlayerState) // ���� PlayerState �� ����������
    {
        return; // ����� �� �������
    }

    Character->SetPlayerColor(PlayerState->GetTeamColor()); // ������������� ���� ������� ���������
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr; // �������� ��������� PlayerState �� ������
    const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr; // �������� ��������� PlayerState �� ������ 

    if (KillerPlayerState) // ���� ������ ����������
    {
        KillerPlayerState->AddKill(); // �� �������� ������� �������� +1
    }

    if (VictimPlayerState) // ���� ������ ����������
    {
        VictimPlayerState->AddDeath(); // �� �������� ������� ������ +1
    }

    StartRespawn(VictimController); // �������� ������� ������ ��������
}

void ASTUGameModeBase::LogPlayerInfo()
{
    if (!GetWorld()) // ���� ���� �� ����������
    {
        return; // ����� �� �������
    }

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) // ���������� � ���� ����������� � ���� ����� �������� ����� �������� GetControllerIterator
    {
        const auto Controller = It->Get(); // ����� ��������� �� ����������
        if (!Controller) // ���� ��������� �� ���������� ��
        {
            continue; // ������� ��������
        }

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState); // ��������� �� PlayerState
        if (!PlayerState) // ���� ��������� �� PlayerState ������� ��
        {
            continue; // ������� �������
        }

        PlayerState->LogInfo(); // �������� ������� ����������� (���������� ������/�������)
    }
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller); // ������� �������� ������ ������
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
    const auto RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime; // ���� ����� ������� �������� ������ ��� MinRoundTimeForRespawn + ����� ��������
    if (!RespawnAvailable) // ���� ������� �� �����������
    {
        return; // ����� �� �������
    }

    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller); // �������� ��������� �� ��������� RespawnComponent
    if (!RespawnComponent) // ���� ��������� �� ����������
    {
        return; // ����� �� �������
    }

    RespawnComponent->Respawn(GameData.RespawnTime); // ����� ������ ������� ������� - ������� ��������� ������ ��������
}

void ASTUGameModeBase::GameOver()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("===== GAME OVER =====")); // ��� ����� ����
    LogPlayerInfo(); // ���������� �������

    for (auto Pawn : TActorRange<APawn>(GetWorld())) // �������� ������ �� ���� ������ � ����
    {
        if (Pawn) // ���� ����� ����
        {
            Pawn->TurnOff(); // ��������� ����������� (���������� ��������, �������� ���������, ���������� ���������)
            Pawn->DisableInput(nullptr); // ���������� ����� ����������
        }
    }
}

