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
};
