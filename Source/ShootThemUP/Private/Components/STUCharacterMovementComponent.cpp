// Shoo Them Up Game


#include "Components/STUCharacterMovementComponent.h"
#include "Player/Public/STUBaseCharacter.h" // ������������ ���� ������ character

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed(); // ����������� ����� ���������� MaxSpeed �������� ������������ �������
    const ASTUBaseCharacter *Player = Cast<ASTUBaseCharacter>(GetPawnOwner()); // ������� ��������� �� ������ character. GetPawnOwner() - ���������� ��������� �� ��������� ������� ����������
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed; // �������� ��� Player �� ������� � ��� �������� �����, � ���� MaxSpeed * RunModifier ���� �� �����, �� ������ ���������� �������� MaxSpeed
}
