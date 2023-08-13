// Shoo Them Up Game


#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Menu/UI/STULevelItemWidget.h"
 
DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton) // ���� ������ ����������
	{
		StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame); // ������������� �� ������� ������� ������
	}

	if (QuitGameButton) // ���� ������ ����������
	{
		QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame); // ������������� �� ������� ������� ������
	}

	InitLevelItems(); // �������� ���� ������ ������ �������
}

void USTUMenuWidget::OnStartGame()
{
	const auto STUGameInstance = GetSTUGameInstance(); // ��������� �� ��� GameInstance
	if (!STUGameInstance) // ���� ��������� �� ����������
	{
		return; // ����� �� �������
	}

	//const FName StartupLevelName = "TestLevel"; // ������ ��������� �� ��� �������
	UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName); // ��������� ��� �������
}

void USTUMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true); // QuitGame(WorldContextObject, ����������, enum, this) - ������� ��������� ��������� ����
}

void USTUMenuWidget::InitLevelItems()
{
	const auto STUGameInstance = GetSTUGameInstance(); // ��������� �� ��� GameInstance
	if (!STUGameInstance) // ���� ��������� �� ����������
	{
		return; // ����� �� �������
	}

	checkf(STUGameInstance->GetLevelsData().Num() != 0, TEXT("Level data must not be empty!")); // ��������� ��� ������ ������� �� ������

	if (!LevelItemsBox) // ���� ��� ���� ������
	{
		return; // ����� �� �������
	}
	LevelItemsBox->ClearChildren(); // ������� ��� �������� �������� � ��

	for (auto LevelData : STUGameInstance->GetLevelsData()) // �������� �� ���� ������ ������� �� �������
	{
		const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass); // ������ ������ ��� ������� �� �������
		if (!LevelItemWidget) // ���� ������ �� ������
		{
			continue; // ���������� �������
		}

		LevelItemWidget->SetLevelData(LevelData); // ���������� ������� ��� �������
		LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected); // ������������� �� ��� ������� ���������

		LevelItemsBox->AddChild(LevelItemWidget); // ��������� � ��� ���� ������ � �������
		LevelItemWidgets.Add(LevelItemWidget); // � ��������� ������ � ������ ��������
	}

	if (STUGameInstance->GetStartupLevel().LevelName.IsNone()) // ���� ��������� ������� �� ����������
	{
		OnLevelSelected(STUGameInstance->GetLevelsData()[0]); // ������������� ������� �� ������
	}
	else
	{
		OnLevelSelected(STUGameInstance->GetStartupLevel()); // ������������� ��������� �������
	}
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
	const auto STUGameInstance = GetSTUGameInstance(); // ��������� �� ��� GameInstance
	if (!STUGameInstance) // ���� ��������� �� ����������
	{
		return; // ����� �� �������
	}
	
	STUGameInstance->SetStartupLevel(Data); // ����� �� ������ �� ���� �� ������ ������ ������ �������� ���������� SetStartupLevel

	for (auto LevelItemWidget : LevelItemWidgets) // ����������� �� ����� ������� �������� � ���������� ����� ������ ��� ������
	{
		if (LevelItemWidget) // ���� � ������� ���� ������
		{
			const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName; // ���� ������� �� �������� ����� �������� ����� �� ������� ��������
			LevelItemWidget->SetSelected(IsSelected); // ����� ������� ����� ����� ������������ ���� �� �������� �� ������ ������ ������
		}
	}
}

USTUGameInstance* USTUMenuWidget::GetSTUGameInstance() const
{
	if (!GetWorld()) // ���� ��� �� ����������
	{
		return nullptr; // ����� �� �������
	}

	return GetWorld()->GetGameInstance<USTUGameInstance>(); // ����� ���������� ��� GameInstance
}
