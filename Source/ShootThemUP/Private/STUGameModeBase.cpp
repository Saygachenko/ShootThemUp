// Shoo Them Up Game

#include "STUGameModeBase.h"
#include "Player/Public/STUBaseCharacter.h"
#include "Player/Public/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass(); // Pawn - ������ ��� ��� Character ����������� �� ����
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots(); // ����� �����

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
            UE_LOG(LogSTUGameModeBase, Display, TEXT("===== GAME OVER =====")); // ��� ����� ����
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
}
