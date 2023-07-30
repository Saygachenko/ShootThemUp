#pragma once

#include "Player/Public/STUPlayerState.h"

class STUUtils
{
  public:
	  template<typename T> 
	  static T* GetSTUPlayerComponent(AActor* PlayerPawn)
	  {
          if (!PlayerPawn) // проверка на pawn
          {
              return nullptr;
          }

          const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass()); // получаем указатель на наш компонент функци€ акторов
                                                   // GetComponentByClass(компонент который мы хотим найти)
          return Cast<T>(Component); // возвращаем каст компонента к типу который мы запрашиваем 
	  }

      bool static AreEnemies(AController* ControllerOne, AController* ControllerTwo) // функци€ проверки €вл€етс€ персонаж противником или нет
      {
          if (!ControllerOne || !ControllerTwo || ControllerOne == ControllerTwo) // если  онтроллера1 или  онтроллера2 или  онтроллер1 равен контроллер2 
          {
              return false; // выход из функции
          }
          const auto PlayerStateOne = Cast<ASTUPlayerState>(ControllerOne->PlayerState); // получаем указатель на PlayerState контроллера
          const auto PlayerStateTwo = Cast<ASTUPlayerState>(ControllerTwo->PlayerState); // получаем указатель на PlayerState контроллера

          return PlayerStateOne && PlayerStateTwo && PlayerStateOne->GetTeaimID() != PlayerStateTwo->GetTeaimID(); // если PlayerStateOne и PlayerStateTwo и PlayerStateOne->GetTeaimID() не равны PlayerStateTwo->GetTeaimID() тогда возвращаем true - это наш враг
      }
};