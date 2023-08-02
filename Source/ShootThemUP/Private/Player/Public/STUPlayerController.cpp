// Shoo Them Up Game


#include "Player/Public/STUPlayerController.h"
#include "Components/STURespawnComponent.h"

ASTUPlayerController::ASTUPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent"); // создаём компонент респавна
}
