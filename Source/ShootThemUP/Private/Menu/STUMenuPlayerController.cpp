// Shoo Them Up Game


#include "Menu/STUMenuPlayerController.h"

void ASTUMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly()); // в данном уровне ввод будет доступен только для интерфейса
	bShowMouseCursor = true; // сделал курсор мыши видимым
}