#pragma once

class STUUtils
{
  public:
	  template<typename T> 
	  static T* GetSTUPlayerComponent(APawn* PlayerPawn)
	  {
          if (!PlayerPawn) // проверка на pawn
          {
              return nullptr;
          }

          const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass()); // получаем указатель на наш компонент функция акторов
                                                   // GetComponentByClass(компонент который мы хотим найти)
          return Cast<T>(Component); // возвращаем каст компонента к типу который мы запрашиваем 
	  }
};