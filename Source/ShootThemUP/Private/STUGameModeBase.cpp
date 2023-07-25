// Shoo Them Up Game

#include "STUGameModeBase.h"
#include "Player/Public/STUBaseCharacter.h"
#include "Player/Public/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"

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
