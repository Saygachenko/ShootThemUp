// Shoo Them Up Game


#include "Components/STUCharacterMovementComponent.h"
#include "Player/Public/STUBaseCharacter.h" // заголовочный файл нашего character

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed(); // присваиваем нашей переменной MaxSpeed значение родительской функции
    const ASTUBaseCharacter *Player = Cast<ASTUBaseCharacter>(GetPawnOwner()); // создали указатель на нашего character. GetPawnOwner() - возвращает указатель на владельца данного компонента
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed; // проверка что Player не нулевой и наш персонаж бежит, и если MaxSpeed * RunModifier если не верно, то просто возвращаем значение MaxSpeed
}
