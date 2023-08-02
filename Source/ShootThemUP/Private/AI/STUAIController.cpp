// Shoo Them Up Game


#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STURespawnComponent.h"

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent); // ��� ����������� ����� ������� ������� AIController - SetPerceptionComponent(������ �� ��������� ���������)

	bWantsPlayerState = true; // ���� PlayerState � �� ��������� �������������

	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent"); // ������ ��������� ��������
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); // ������ begin play

	const auto STUCharacter = Cast<ASTUAICharacter>(InPawn); // ���� ����� ������� ��������� ������� OnPossess � ������ ASTUAICharacter
	if (STUCharacter) // ���� true
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset); // ��������� ������ ��������� RunBehaviorTree(��������� �� ������ ���������)
	}
}

void ASTUAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = GetFocusOnActor(); // ���������� ����������� ��������� ������� GetFocusOnActor - ����������� �� ���������� ������
	SetFocus(AimActor); // SetFocus() - ������� ���������� �������� ����������� ����� �������, ����� FVector �������� �� ����� ������� �� ��������� �� � ���������
}

AActor* ASTUAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) // ���� GetBlackboardComponent ���������� false
	{
		return nullptr; // ���������� ��� �� 0 
	}
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName)); // ���������� ������ �� ������� ��� ����� ���������������
}
