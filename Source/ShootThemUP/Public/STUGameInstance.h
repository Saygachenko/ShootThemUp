// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FString TestString = "Hello Game!"; // ���� ��� �����

	FName GetStartupLevelName() const // �������� ��� ��������� ������
	{
		return StartupLevelName;
	}

	FName GetMenuLevelName() const // �������� ��� ������ ����
	{
		return MenuLevelName;
	}

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName StartupLevelName = NAME_None; // ���� ������ �������� ������ (���� ��� none, ������� ���� �� ������)

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName MenuLevelName = NAME_None; // ���� ������ �������� ������ (���� ��� none, ������� ���� �� ������)
	
};
