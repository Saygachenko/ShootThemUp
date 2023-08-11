// Shoo Them Up Game


#include "Menu/STUMenuPlayerController.h"
#include "STUGameInstance.h"

void ASTUMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly()); // � ������ ������ ���� ����� �������� ������ ��� ����������
	bShowMouseCursor = true; // ������ ������ ���� �������

	GetWorld()->GetGameInstance<USTUGameInstance>()->TestString = "Menu Level says hello!"; // �������� ��������� �� ��� GameIstance (���� ������)
}