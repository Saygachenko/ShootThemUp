// Shoo Them Up Game


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/STUHealthBarWidget.h"
#include "Components/STUHealthComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer &ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent")) // Функция SetDefaultSubobjectClass для замены компонента по умолчанию
{
    AutoPossessAI = EAutoPossessAI::Disabled; // пешка автоматически переходит во владение AI-контроллера при каждом ее создании
    AIControllerClass = ASTUAIController::StaticClass(); // установили ИИ базовый контроллер

    bUseControllerRotationYaw = false; // RotationYaw выкл
    if (GetCharacterMovement()) // если true
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true; // ControllerDesiredRotation = true
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f); // изменяем скорость поворота
    }

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent"); // создаём виджет компонент
    HealthWidgetComponent->SetupAttachment(GetRootComponent()); // присоединяем к коренному компоненту
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // чтобы бар был виден через всю карту
    HealthWidgetComponent->SetDrawAtDesiredSize(true); // устанавливаем true чтобы мы видели наш бар в игре
}

void ASTUAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateHealthWidgetVisibility(); // каждый тик проверяем нашу функцию
}

void ASTUAICharacter::BeginPlay()
{
   Super::BeginPlay();
   check(HealthWidgetComponent); // проверка нашего виджет компонента здоровья
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath(); // вызываем базовую функцию OnDeath()

    const auto STUController = Cast<AAIController>(Controller); // указатель на контроллера ИИ
    if (STUController && STUController->BrainComponent) // если есть указатель на ИИ контроллер и Мозговой компонент
    {
        STUController->BrainComponent->Cleanup(); // вызываем функцию остановки дерева поведения сразу же после смерти игрока
    }
}

void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta)
{
    Super::OnHealthChanged(Health, HealthDelta); // вызываем базовую функцию ХП

    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject()); // указатель на наш виджет бар здоровья
    if (!HealthBarWidget) // если указателя не сущетствует
    {
        return; // выход из функции
    }

    HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent()); // устанавливаем процент ХП нашему бару
}

void ASTUAICharacter::UpdateHealthWidgetVisibility()
{
    if (!GetWorld() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()) // если указатель на мир или указатель на контроллер игрока или указатель на игрока не существуют
    {
        return; // выход из функции
    }

    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation(); // указатель на местоположения контроллера (игрока). если возвращать GetPawnm() - то бар будет оставаться видимым от последнего условия (близко мы были к ИИ или нет), через GetPawnOrSpectator() проблема решается
    const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation()); // получаем указатель на дистанцию между игроком и ИИ
    HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true); // если дистанция игрока меньше чем заданная в HealthVisibilityDistance то нам виден бар ХП
}
