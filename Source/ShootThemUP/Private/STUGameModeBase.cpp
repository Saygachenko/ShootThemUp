// Shoo Them Up Game

#include "STUGameModeBase.h"
#include "Player/Public/STUBaseCharacter.h"
#include "Player/Public/STUPlayerController.h"
#include "UI/STUGameHUD.h"

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass(); // Pawn - потому что наш Character наследуется от него
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
}
