// Shoo Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUHealthBarWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHealthPercent(float Percent); // функция передаёт текущий процент ХП

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar; // проперти нашего бара ХП

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float PercentVisibilityThreshold = 0.8; // проперти кол-во хп при котором будет видимым наш БАР ХП

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float PercentColorThreshold = 0.3f; // проперти отвечает за цвет бара в зависимости от кол-во хп

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor GoodColor = FLinearColor::White; // проперти отвечает за цвет нашего бара

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor BadColor = FLinearColor::Red; // проперти отвечает за цвет нашего бара
};
