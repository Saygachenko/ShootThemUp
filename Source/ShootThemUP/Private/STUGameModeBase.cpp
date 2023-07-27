// Shoo Them Up Game

#include "STUGameModeBase.h"
#include "Player/Public/STUBaseCharacter.h"
#include "Player/Public/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

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

    CurrentRound = 1; // текущий раунд
    StartRound(); // старт раунда
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

void ASTUGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime; // обновляем переменную которая отвечает за время раунда
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true); // запускаем зацикленный таймер, вызывается каждую секунду
}

void ASTUGameModeBase::GameTimerUpdate()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i / Round: %i / %i"), RoundCountDown, CurrentRound, GameData.RoundsNum); // лог времени раунда, текущий раунд и макс раундов

    if (--RoundCountDown == 0) // уменьшаем кол-во времени до конца на 1 и если оно равно 0
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle); // удаляем наш таймер
        if (CurrentRound + 1 <= GameData.RoundsNum) // текущий раунд +1 и если он меньше или равно макс числу раундов
        {
            ++CurrentRound; // текущий раунд + 1
            ResetPlayers(); // рестарт игроков
            StartRound(); // старт нового раунда
        }
        else
        {
            UE_LOG(LogSTUGameModeBase, Display, TEXT("===== GAME OVER =====")); // лог конца игры
        }
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    if (!GetWorld()) // если мира не существует
    {
        return; // выход из фукнции
    }

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) // информацию о всех котроллерах в мире можно получить через итератор GetControllerIterator
    {
        ResetOnePlayer(It->Get()); // вызываем для каждого контроллера на сцене ResetOnePlayer. Для получения сырого укозателя вызываем функцию Get()
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn()) // если контроллер и павн который им владеет существуют
    {
        Controller->GetPawn()->Reset(); // вызываем функцию Reset() - рестарт
    }
    RestartPlayer(Controller); // рестарт персонажа
}
