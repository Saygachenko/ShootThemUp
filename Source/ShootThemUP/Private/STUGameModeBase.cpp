// Shoo Them Up Game

#include "STUGameModeBase.h"
#include "Player/Public/STUBaseCharacter.h"
#include "Player/Public/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass(); // Pawn - потому что наш Character наследуется от него
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots(); // спавн ботов
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>()) // проверяем являится-ли данный котроллер ИИ контроллером
    {
        return AIPawnClass; // возвращаем нашего персонажа
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController); // иначе возвращаем родительскую функцию
}

void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld()) // если указатель на мир не существует
    {
        return; // выход из функции
    }

    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i) // цикл фор от 0 до GameData.PlayersNum -1 потому что помимо ботов существует наш персонаж
    {
        FActorSpawnParameters SpawnInfo; // FActorSpawnParameters в ней определяем как должны обрабатываться коллизии при спавне наих игроков
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn обрабатываем каким образум будут обрабатываться коллизии. AlwaysSpawn - спавн всегда

        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo); // спавним актора SpawnActor<AAIController>(Класс контроллера который находится в проперти, структура спавна коллизии);
        RestartPlayer(STUAIController); // RestartPlayer(базовый класс контроллера) - создаёт павна
    }
}
