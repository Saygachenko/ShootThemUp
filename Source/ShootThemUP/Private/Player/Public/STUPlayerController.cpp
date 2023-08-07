// Shoo Them Up Game


#include "Player/Public/STUPlayerController.h"
#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

ASTUPlayerController::ASTUPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent"); // ������ ��������� ��������
}

void ASTUPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()) // ���� ��������� �� ��� ����������
	{
		const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()); // �������� ��������� �� �������
		if (GameMode) // ���� ��������� ����������
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged); // ������������� �� ��� ������� ��������� ����
		}
	}
}

void ASTUPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnNewPawn.Broadcast(InPawn); // ���� �������� ������� OnNewPawn
}

void ASTUPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent(); // ����� ������������ �������
	if (!InputComponent) // ���� ����� ���������� �� ����������
	{
		return; // ����� �� �������
	}

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUPlayerController::OnPauseGame); // ���� ������ ����� �� ESC
}

void ASTUPlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) // ���� ��� �� ���������� ��� �� ���������� GameMode
	{
		return; // ����� �� �������
	}

	GetWorld()->GetAuthGameMode()->SetPause(this); // SetPause(���������� ������� ����� ��������� �����, ������� ����������� ����� �� ������� �� ����� ���� ��� ���) - ������� ������ ���� �� �����
}

void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State)
{
	if (State == ESTUMatchState::InProgress) // ���� �� ��������� � �������� ����
	{
		SetInputMode(FInputModeGameOnly()); // ��������� ���� ��� ������ SetInputMode()
		bShowMouseCursor = false; // ������ ���������� �� �����
	}
	else
	{
		SetInputMode(FInputModeUIOnly()); // ��������� ���� ��� ������ SetInputMode()
		bShowMouseCursor = true; // �� ���� ������ ������� ����������
	}
}
