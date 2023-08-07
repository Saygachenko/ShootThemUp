// Shoo Them Up Game


#include "Player/Public/STUPlayerController.h"
#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

ASTUPlayerController::ASTUPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent"); // создаём компонент респавна
}

void ASTUPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()) // если указатель на мир существует
	{
		const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()); // получаем указатель на гейммод
		if (GameMode) // если указатель существует
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged); // подписываемся на наш делегат состояния игры
		}
	}
}

void ASTUPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnNewPawn.Broadcast(InPawn); // явно вызываем делегат OnNewPawn
}

void ASTUPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent(); // вызов родительской функции
	if (!InputComponent) // если инпут компонента не существует
	{
		return; // выход из функции
	}

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUPlayerController::OnPauseGame); // бинд кнопки паузы на ESC
}

void ASTUPlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) // если мир не существует или не существует GameMode
	{
		return; // выход из функции
	}

	GetWorld()->GetAuthGameMode()->SetPause(this); // SetPause(контроллер который хочет поставить паузу, делегат определения можем мы ставить на паузу игру или нет) - функция ставит игру на паузу
}

void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State)
{
	if (State == ESTUMatchState::InProgress) // если мы находимся в процессе игры
	{
		SetInputMode(FInputModeGameOnly()); // указываем явно вид инпута SetInputMode()
		bShowMouseCursor = false; // курсор показывать не нужно
	}
	else
	{
		SetInputMode(FInputModeUIOnly()); // указываем явно вид инпута SetInputMode()
		bShowMouseCursor = true; // во всех других случаях показываем
	}
}
