// Shoo Them Up Game

#include "STUGameModeBase.h"
#include "Player/Public/STUBaseCharacter.h"
#include "Player/Public/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"
#include "Player/Public/STUPlayerState.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10; // константая переменная минимальное время респавна до окончания раунда

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass(); // Pawn - потому что наш Character наследуется от него
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass(); // указываем PlayerState статическим
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots(); // спавн ботов
    CreateTeamsInfo(); // функция для распределения игроков по командам

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
            GameOver(); // функция окончания игры
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
        ResetOnePlayer(It->Get()); // вызываем для каждого контроллера на сцене ResetOnePlayer. Для получения сырого указателя вызываем функцию Get()
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn()) // если контроллер и павн который им владеет существуют
    {
        Controller->GetPawn()->Reset(); // вызываем функцию Reset() - рестарт
    }
    RestartPlayer(Controller); // рестарт персонажа
    SetPlayerColor(Controller); // установка цвета команды
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld()) // если мира не существует
    {
        return; // выход из фукнции
    }

    int32 TeamID = 1; // переменная номера команды
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) // информацию о всех котроллерах в мире можно получить через итератор GetControllerIterator
    {
        const auto Controller = It->Get(); // сырой указатель на контроллер
        if (!Controller) // если указатель не существует то
        {
            continue; // пропуск условаия
        }

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState); // указатель на PlayerState
        if (!PlayerState) // если указатель на PlayerState нулевой то
        {
            continue; // пропуск условия
        }

        PlayerState->SetTeaimID(TeamID); // устанавливаем тим ИД
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID)); // устанавливаем цвет команде

        SetPlayerColor(Controller); // установка цвета команды

        TeamID = TeamID == 1 ? 2 : 1; // если тим ID равен 1 команде то меняем на 2 и наоборот
    }
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num()) // если тим ИД меньше чем кол-во цветов(тим ид начинается 1 , в массиве начинается с 0)
    {
        return GameData.TeamColors[TeamID - 1]; // возвращаем цвет с идексом тим ид
    }
    UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id: %i, set to default: %s"), TeamID, *GameData.DefaultTeamColor.ToString()); // если забыли установить цвета в массив 
    return GameData.DefaultTeamColor; // возвращаем дефолтный красный цвет команде
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller) // если контроллера не существует
    {
        return; // выход из функции
    }

    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn()); // указатель на character
    if (!Character) // если указатель на персонажа не существует
    {
        return; // выход из функции
    }

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState); // указатель на PlayerState
    if (!PlayerState) // если PlayerState не существует
    {
        return; // выход из функции
    }

    Character->SetPlayerColor(PlayerState->GetTeamColor()); // устанавливаем цвет команды персонажу
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr; // получаем указатель PlayerState на убийцу
    const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr; // получаем указатель PlayerState на жертву 

    if (KillerPlayerState) // если убийца существует
    {
        KillerPlayerState->AddKill(); // то вызываем функции убийства +1
    }

    if (VictimPlayerState) // если жертва существует
    {
        VictimPlayerState->AddDeath(); // то вызываем функции смерти +1
    }

    StartRespawn(VictimController); // вызываем функция старта респавна
}

void ASTUGameModeBase::LogPlayerInfo()
{
    if (!GetWorld()) // если мира не существует
    {
        return; // выход из функции
    }

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It) // информацию о всех котроллерах в мире можно получить через итератор GetControllerIterator
    {
        const auto Controller = It->Get(); // сырой указатель на контроллер
        if (!Controller) // если указатель не существует то
        {
            continue; // пропуск условаия
        }

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState); // указатель на PlayerState
        if (!PlayerState) // если указатель на PlayerState нулевой то
        {
            continue; // пропуск условия
        }

        PlayerState->LogInfo(); // вызываем уровень логирования (статистика убийст/смертей)
    }
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller); // функция рестарта одного игрока
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
    const auto RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime; // если время которое осталось больше чем MinRoundTimeForRespawn + время респавна
    if (!RespawnAvailable) // если условие не выполняется
    {
        return; // выход из функции
    }

    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller); // получаем указатель на компонент RespawnComponent
    if (!RespawnComponent) // если указатель не существует
    {
        return; // выход из функции
    }

    RespawnComponent->Respawn(GameData.RespawnTime); // иначе запуск функции Респавн - которая запускает таймер респавна
}

void ASTUGameModeBase::GameOver()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("===== GAME OVER =====")); // лог конца игры
    LogPlayerInfo(); // статистика игроков

    for (auto Pawn : TActorRange<APawn>(GetWorld())) // проходим циклом по всем павнам в мире
    {
        if (Pawn) // если павны есть
        {
            Pawn->TurnOff(); // остановка персонайжей (отключение коллизии, движения персонажа, физических симуляций)
            Pawn->DisableInput(nullptr); // отключение ввода клавиатуры
        }
    }
}

