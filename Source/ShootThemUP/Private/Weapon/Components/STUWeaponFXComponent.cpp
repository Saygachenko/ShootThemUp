// Shoo Them Up Game


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h" // находятся всё необходимые функции для спавна ниагары
#include "PhysicalMaterials/PhysicalMaterial.h" // библиотека PhysicalMaterials
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
    auto ImpactData = DefaultImpactData; // присвоили ImpactData - эффект по умолчанию

	if (Hit.PhysMaterial.IsValid()) // информация о физическом материале PhysMaterial.IsValid() если true
	{
        const auto PhysMat = Hit.PhysMaterial.Get(); // то мы можем получить доступ к сырому указателю Get() - возвращает указатель на физический материал
		if (ImpactDataMap.Contains(PhysMat)) // проверка на существование специального эффекта для данного физического материала Contains - проверяет существования ключа который мы в неё передали
		{
            ImpactData = ImpactDataMap[PhysMat]; // переопределяем эффект который мы хотим заспавнить
		}
	}
	// spawn Niagar
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.NiagaraEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation()); // SpawnSystemAtLocation(указатель на мир или объект, указатель на ниагар систему, локация в мире, ориентация в пространстве) - спавнит ниагару в мир
	// spawn decal
	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImpactData.DecalData.Material, ImpactData.DecalData.Size, Hit.ImpactPoint, Hit.ImpactNormal.Rotation()); // спавн декаля SpawnDecalAtLocation(указатель на мир, материал декаля, размер декаля, локация в мире декаля, ориентация в пространстве)
	if (DecalComponent) // проверка что компонент заспавнен
	{
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime); // устанавливаем параметр жизни декаля SetFadeOut(через какое время запустится анимация, время анимации исчезания)
	}
}
