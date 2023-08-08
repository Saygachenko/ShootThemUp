#pragma once

#include "Player/Public/STUPlayerState.h"

class STUUtils
{
  public:
	  template<typename T> 
	  static T* GetSTUPlayerComponent(AActor* PlayerPawn)
	  {
          if (!PlayerPawn) // �������� �� pawn
          {
              return nullptr;
          }

          const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass()); // �������� ��������� �� ��� ��������� ������� �������
                                                   // GetComponentByClass(��������� ������� �� ����� �����)
          return Cast<T>(Component); // ���������� ���� ���������� � ���� ������� �� ����������� 
	  }

      bool static AreEnemies(AController* ControllerOne, AController* ControllerTwo) // ������� �������� �������� �������� ����������� ��� ���
      {
          if (!ControllerOne || !ControllerTwo || ControllerOne == ControllerTwo) // ���� �����������1 ��� �����������2 ��� ����������1 ����� ����������2 
          {
              return false; // ����� �� �������
          }
          const auto PlayerStateOne = Cast<ASTUPlayerState>(ControllerOne->PlayerState); // �������� ��������� �� PlayerState �����������
          const auto PlayerStateTwo = Cast<ASTUPlayerState>(ControllerTwo->PlayerState); // �������� ��������� �� PlayerState �����������

          return PlayerStateOne && PlayerStateTwo && PlayerStateOne->GetTeamID() != PlayerStateTwo->GetTeamID(); // ���� PlayerStateOne � PlayerStateTwo � PlayerStateOne->GetTeaimID() �� ����� PlayerStateTwo->GetTeaimID() ����� ���������� true - ��� ��� ����
      }

      static FText TextFromInt(int32 Num) // ������� ������� ��������������� ��� � �����
      {
          return FText::FromString(FString::FromInt(Num));
      }
};