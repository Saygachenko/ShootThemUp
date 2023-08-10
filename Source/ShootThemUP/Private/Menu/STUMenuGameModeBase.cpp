// Shoo Them Up Game


#include "Menu/STUMenuGameModeBase.h"
#include "Menu/STUMenuPlayerController.h"
#include "Menu/UI/STUMenuHUD.h"

ASTUMenuGameModeBase::ASTUMenuGameModeBase()
{
	PlayerControllerClass = ASTUMenuPlayerController::StaticClass(); // контроллер делаем статическим к данному GameModeBase
	HUDClass = ASTUMenuHUD::StaticClass(); // класс худ мы делаем статическим к данному GameModeBase
}
