// Shoo Them Up Game


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"

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
