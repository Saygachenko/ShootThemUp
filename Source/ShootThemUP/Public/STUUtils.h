#pragma once

class STUUtils
{
  public:
	  template<typename T> 
	  static T* GetSTUPlayerComponent(APawn* PlayerPawn)
	  {
          if (!PlayerPawn) // �������� �� pawn
          {
              return nullptr;
          }

          const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass()); // �������� ��������� �� ��� ��������� ������� �������
                                                   // GetComponentByClass(��������� ������� �� ����� �����)
          return Cast<T>(Component); // ���������� ���� ���������� � ���� ������� �� ����������� 
	  }
};