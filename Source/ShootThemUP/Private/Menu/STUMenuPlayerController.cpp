// Shoo Them Up Game


#include "Menu/STUMenuPlayerController.h"
#include "STUGameInstance.h"

void ASTUMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly()); // в данном уровне ввод будет доступен только для интерфейса
	bShowMouseCursor = true; // сделал курсор мыши видимым

	GetWorld()->GetGameInstance<USTUGameInstance>()->TestString = "Menu Level says hello!"; // получаем указатель на наш GameIstance (тест строка)
}