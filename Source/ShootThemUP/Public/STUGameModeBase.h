// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

class AAIController; // деклорация котроллера ИИ

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    ASTUGameModeBase();

    virtual void StartPlay() override; // старт игры, вызывается до BeginPlay(), GameMode и всех остальных акторов (тут мы спавним ботов)

    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override; // функция которая возвращает класс Павн который необходимо заспавнить

    void Killed(AController* KillerController, AController* VictimController); // функция подсчёта статистики убийств/смертей

    FGameData GetGameData() const // гетер нашей структуры
    {
        return GameData;
    }
    int32 GetCurrentRoundNum() const // геттер текущего раунда
    {
        return CurrentRound;
    }
    int32 GetRoundSecondsRemaining() const // геттер времени текущего раунда
    {
        return RoundCountDown;
    }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass; // указатель на класс контроллера ИИ

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass; // указатель на класс ИИ персонажа

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData; // указатель на нашу структуру

private:
    int32 CurrentRound = 1; // номер текущего раунда
    int32 RoundCountDown = 0; // кол-во времени которое осталось для текущего раунда
    FTimerHandle GameRoundTimerHandle;

    void SpawnBots(); // функция логики спавна ботов
    void StartRound(); // функция старта раунда
    void GameTimerUpdate(); // функция обновления таймера

    void ResetPlayers(); // функция рестарта всех игроков
    void ResetOnePlayer(AController* Controller); // функция рестарта одного игрока

    void CreateTeamsInfo(); // функция распределение игроков по командам (вызывается 1 раз при старте игры)
    FLinearColor DetermineColorByTeamID(int32 TeamID) const;// функция возвращает цвет взависимости от team ID
    void SetPlayerColor(AController* Controller); // функция будет передавать классу Character его цвет

    void LogPlayerInfo(); // функция печатает инфу в логи про всех PlayerState
};
