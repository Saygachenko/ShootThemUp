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

          const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass()); // получаем указатель на наш компонент функция акторов
                                                   // GetComponentByClass(компонент который мы хотим найти)
          return Cast<T>(Component); // возвращаем каст компонента к типу который мы запрашиваем 
	  }

      bool static AreEnemies(AController* ControllerOne, AController* ControllerTwo) // функция проверки является персонаж противником или нет
      {
          if (!ControllerOne || !ControllerTwo || ControllerOne == ControllerTwo) // если Контроллера1 или Контроллера2 или Контроллер1 равен контроллер2 
          {
              return false; // выход из функции
          }
          const auto PlayerStateOne = Cast<ASTUPlayerState>(ControllerOne->PlayerState); // получаем указатель на PlayerState контроллера
          const auto PlayerStateTwo = Cast<ASTUPlayerState>(ControllerTwo->PlayerState); // получаем указатель на PlayerState контроллера

          return PlayerStateOne && PlayerStateTwo && PlayerStateOne->GetTeamID() != PlayerStateTwo->GetTeamID(); // если PlayerStateOne и PlayerStateTwo и PlayerStateOne->GetTeaimID() не равны PlayerStateTwo->GetTeaimID() тогда возвращаем true - это наш враг
      }

      static FText TextFromInt(int32 Num) // функция которая преобразовывает инт в текст
      {
          return FText::FromString(FString::FromInt(Num));
      }
};