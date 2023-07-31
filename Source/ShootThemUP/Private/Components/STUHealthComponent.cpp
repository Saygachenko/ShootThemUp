// Shoo Them Up Game


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h" // ���������� ������ ������
//#include "Dev/STUFireDamageType.h" // ��� ��� ����� �����
//#include "Dev/STUIceDamageType.h" // ��� ��� ����� ��
#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // � ������ ������ ��� �� ����������� ���


}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0); // ����� �� ���� ������� �� 0 � ������������� ��������

	SetHealth(MaxHealth); // ������ ��� ����� ���� ����� �����������, �� � ��������� ����� 100

	AActor *ComponentOwner = GetOwner(); // GetOwner ���������� �� ��������� ������� ����������
	if (ComponentOwner) // ���� ���� �������� � ������� ���������� (�� �������)
	{
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage); // ����������� �� ������� OnTakeAnyDamage
	}

	
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld()) // ���� ���� ������ ��� ����� 0, ���� �� ��� ������, ���� �� ���
	{
        return;
	}

	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle); // ���� �� ������ �������� ����, ������� ��� ������ ��������

	if (IsDead()) // ��� ���� ��������, ���� �� ��� ���
	{
		Killed(InstigatedBy); // ������� ������
        OnDeath.Broadcast(); // ����� ������ ��������, ����� ������� �� ��������� ���� �������� ������� ��������� �� ������� � ������
	}
    else if (AutoHeal) // ���� �� ���� �� ����������� �������
	{
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}

	PlayCameraShake(); // �������� ������ ������ ��� ��������� �����
    //UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);

	/*if (DamageType) ��� �����
	{
		if (DamageType->IsA<USTUFireDamageType>()) // ���������� ������� UObject IsA<��������� ��� �������>(���������� true ���� ������ ������ ��������� � ����� ����)
		{
            UE_LOG(LogHealthComponent, Display, TEXT("Fire Damage"));
		}
		else if (DamageType->IsA<USTUIceDamageType>())
		{
            UE_LOG(LogHealthComponent, Display, TEXT("Ice Damage"));
		}
	}*/
}

void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier); // ���������� ������ ��

	if (IsHealthFull() && GetWorld()) // �������� �� �� ��� ������� �� ����� �������������. ����� � ��������� ������
                                    // ��������� ����� ������� IsNearlyEqual()
	{
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle); // ���� ��� �����, ������� ��� ������
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth); // ����������� ������������ � ������������� ��. ������� Clamp(������� ����������, ������� �������, ������� �������)
    const auto HealthDelta = NextHealth - Health; // ���������� ������� ����� ���� �� � �������

    Health = NextHealth; // Health ����������� NextHealth
    OnHealthChanged.Broadcast(Health, HealthDelta); // ���������� ���� �������� ��� ���� �� ���������� HealthDelta - �������� ��������� ��������
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsDead() || IsHealthFull()) // ���� �������� ����, ��� ������ ����� ��������
	{
        return false;
	}
    SetHealth(Health + HealthAmount); // ��������� ������������ ����� ������ � ��������
    return true;
}

bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth); // ���� ������� �� ����� ������������� �� ���������� true
}

void USTUHealthComponent::PlayCameraShake()
{
	if (IsDead()) // �������� ��� ��� �������� ����
	{
        return;
	}

	const auto Player = Cast<APawn>(GetOwner()); // ��������� �� ����� (� �� ���� ��������� �� ���������)
	if (!Player) // ���� ���������� ����� �� ���������
	{
        return;
	}

	const auto Controller = Player->GetController<APlayerController>(); // �������� ��������� �� ����������
	if (!Controller || !Controller->PlayerCameraManager) // ���� ��������� ������� ����, ��� ��������� PlayerCameraManager �������
	{
        return;
	}

	Controller->PlayerCameraManager->StartCameraShake(CameraShake); // �������� ������� StartCameraShake

}

void USTUHealthComponent::Killed(AController* KillerController)
{
	if (!GetWorld()) // ���� ��� �� ����������
	{
		return; // ����� �� �������
	}

	const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()); // ��������� �� ���� ���
	if (!GameMode) // ���� ��������� �� ���� ��� �� ���������
	{
		return; // ����� �� ��������
	}

	const auto Player = Cast<APawn>(GetOwner()); // ��������� �� ������ Character
	const auto VictimController = Player ? Player->Controller : nullptr; // ���� ��� �������� ����������, �������� ��� ���������� ����� ����������� nullptr

	GameMode->Killed(KillerController, VictimController); // �������� ������� ����
}
