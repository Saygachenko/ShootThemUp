// Shoo Them Up Game


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h" // ��������� �� ����������� ������� ��� ������ �������
#include "PhysicalMaterials/PhysicalMaterial.h" // ���������� PhysicalMaterials
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

// Sets default values for this component's properties
USTUWeaponFXComponent::USTUWeaponFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult &Hit)
{
    auto ImpactData = DefaultImpactData; // ��������� ImpactData - ������ �� ���������

	if (Hit.PhysMaterial.IsValid()) // ���������� � ���������� ��������� PhysMaterial.IsValid() ���� true
	{
        const auto PhysMat = Hit.PhysMaterial.Get(); // �� �� ����� �������� ������ � ������ ��������� Get() - ���������� ��������� �� ���������� ��������
		if (ImpactDataMap.Contains(PhysMat)) // �������� �� ������������� ������������ ������� ��� ������� ����������� ��������� Contains - ��������� ������������� ����� ������� �� � �� ��������
		{
            ImpactData = ImpactDataMap[PhysMat]; // �������������� ������ ������� �� ����� ����������
		}
	}
	// spawn Niagar
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.NiagaraEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation()); // SpawnSystemAtLocation(��������� �� ��� ��� ������, ��������� �� ������ �������, ������� � ����, ���������� � ������������) - ������� ������� � ���
	// spawn decal
	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImpactData.DecalData.Material, ImpactData.DecalData.Size, Hit.ImpactPoint, Hit.ImpactNormal.Rotation()); // ����� ������ SpawnDecalAtLocation(��������� �� ���, �������� ������, ������ ������, ������� � ���� ������, ���������� � ������������)
	if (DecalComponent) // �������� ��� ��������� ���������
	{
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime); // ������������� �������� ����� ������ SetFadeOut(����� ����� ����� ���������� ��������, ����� �������� ���������)
	}
}
