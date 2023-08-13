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

	if (StartGameButton) // если кнопка существует
	{
		StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame); // подписываемся на делегат нажатия кнопки
	}

	if (QuitGameButton) // если кнопка существует
	{
		QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame); // подписываемся на делегат нажатия кнопки
	}

	InitLevelItems(); // вызываем нашу логику клеток уровней
}

void USTUMenuWidget::OnStartGame()
{
	const auto STUGameInstance = GetSTUGameInstance(); // указатель на наш GameInstance
	if (!STUGameInstance) // если указателя не существует
	{
		return; // выход из функции
	}

	//const FName StartupLevelName = "TestLevel"; // делаем указатель на наш уровень
	UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName); // загружаем наш уровень
}

void USTUMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true); // QuitGame(WorldContextObject, контроллер, enum, this) - функция позволяет завершить игру
}

void USTUMenuWidget::InitLevelItems()
{
	const auto STUGameInstance = GetSTUGameInstance(); // указатель на наш GameInstance
	if (!STUGameInstance) // если указателя не существует
	{
		return; // выход из функции
	}

	checkf(STUGameInstance->GetLevelsData().Num() != 0, TEXT("Level data must not be empty!")); // проверяем что массив уровней не пустой

	if (!LevelItemsBox) // если наш бокс пустой
	{
		return; // выход из функции
	}
	LevelItemsBox->ClearChildren(); // удаляем все дочерние элементы в нём

	for (auto LevelData : STUGameInstance->GetLevelsData()) // проходим по всем данным уровней из массива
	{
		const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass); // создаём виджет для уровней из массива
		if (!LevelItemWidget) // если виджет не создан
		{
			continue; // пропускаем условие
		}

		LevelItemWidget->SetLevelData(LevelData); // установили уровень для виджета
		LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected); // подписываемся на наш делегат структуры

		LevelItemsBox->AddChild(LevelItemWidget); // добавляем в наш бокс виджет с уровнем
		LevelItemWidgets.Add(LevelItemWidget); // и добавляем виджет в массив виджетов
	}

	if (STUGameInstance->GetStartupLevel().LevelName.IsNone()) // если начальный уровень не установлен
	{
		OnLevelSelected(STUGameInstance->GetLevelsData()[0]); // устанавливаем уровень на прямую
	}
	else
	{
		OnLevelSelected(STUGameInstance->GetStartupLevel()); // устанавливаем начальный уровень
	}
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
	const auto STUGameInstance = GetSTUGameInstance(); // указатель на наш GameInstance
	if (!STUGameInstance) // если указателя не существует
	{
		return; // выход из функции
	}
	
	STUGameInstance->SetStartupLevel(Data); // когда мы нажали на одну из кнопок уровня меняем значение переменной SetStartupLevel

	for (auto LevelItemWidget : LevelItemWidgets) // пробегаемся по всеми массиву виджетов и определяем какой сейчас был выбран
	{
		if (LevelItemWidget) // если в массиве есть уровни
		{
			const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName; // если уровень из делегата равен текущему имени из массива виджетов
			LevelItemWidget->SetSelected(IsSelected); // таким образом рамка будет отображаться если мы кликнула на данный виджет уровня
		}
	}
}

USTUGameInstance* USTUMenuWidget::GetSTUGameInstance() const
{
	if (!GetWorld()) // если мир не существует
	{
		return nullptr; // выход из функции
	}

	return GetWorld()->GetGameInstance<USTUGameInstance>(); // иначе возвращаем наш GameInstance
}
