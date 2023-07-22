// Shoo Them Up Game


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h" // ���������� ���������
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

    checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);

    InitAnimations();
	SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr; // CurrentWeapon = 0
    for (auto Weapon : Weapons) // ����������� �� ������� ������
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); // ����������� ������� ������ �� ����
        Weapon->Destroy(); // ������� ������ ������
    }
    Weapons.Empty(); // �������� ����-�� �������� � �������

    Super::EndPlay(EndPlayReason); // ����� EndPlay
}

void USTUWeaponComponent::SpawnWeapons()
{

    ACharacter *Character = Cast<ACharacter>(GetOwner()); // ��������� �� GetOwner()
    if (!Character || !GetWorld()) // �������� �� Owner � ��� ����
    {
        return;
    }

    for (auto OneWeaponData : WeaponData) // ���������� �� ����� ������� WeaponData
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass); // ������� ������ � ���� ����. ������� SpawnActor ���������� ��������� �� ��������� ������
        if (!Weapon) // ���� ������ ����������
        {
            continue;
        }
        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
        Weapon->SetOwner(Character); // ��������� ��������� ������ ������
        Weapons.Add(Weapon); // ��������� � Weapons ��������� �� ���������

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent) 
    {
        return;
    }
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false); // AttachmentRules(EAttachmentRule, ��������� ���������� ���)
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName); // ������������ ������ � ������.
                                                            // AttachToComponent(��������� �� ��������� ������� �� ����� ������������ � ������� ���������, AttachmentRules, �������� ������� �������)
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num()) // �������� �� ������ ������, ������ ���� ������ ���� ���� ������ ���-�� ��������� �������
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
    }

    ACharacter *Character = Cast<ACharacter>(GetOwner()); // ��������� �� GetOwner()
    if (!Character)
    {
        return;
    }
    if (CurrentWeapon) // CurrentWeapon ����� ������� ������ ��� ������ � BeginPlay
    {
        CurrentWeapon->StopFire(); // ������������� �������� ��� ����� ������
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName); // ���������� ������ �� ����� ���������
    }

    CurrentWeapon = Weapons[WeaponIndex]; // ������ �������� ������
                                                                                               // �������� ������� Data ����� ��� �������� �������
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData &Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); }); // ������ ��� ����� ������ � ������� WeaponData ��������� ������� ������������� ������ ������ ������ � ���� ������� ���� �� ���� �������� �����������
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr; // �������� �� 0

    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName); // ����������� ������ � ���� ���������

    EquipAnimInProgress = true;

    PlayAnimMontag(EquipAnimMontage); // �������� �������� EquipAnimMontage, ����� ��� ������������� ��� ����� ������
}

void USTUWeaponComponent::StartFire() // ������� ����
{
    if (!CanFire()) // ���� �� �� ����� ��������, �� ������� �� �������
    {
        return;
    }
    CurrentWeapon->StartFire(); // ���� ����, �������� ������� Fire
}

void USTUWeaponComponent::StopFire() // ������� ����
{
    if (!CurrentWeapon) // ���� ������ ������, �� ������� �� �������
    {
        return;
    }
    CurrentWeapon->StopFire(); // ���� ����, �������� ������� Fire
}

void USTUWeaponComponent::NextWeapon()
{
    if (!CanEquip()) // ���� �� �� ����� ������ ������, �� ������ ������� �� �������
    {
        return;
    }
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num(); // ����������� CurrentWeaponIndex �� 1 � ����� ���������� �� ����� �� ������� ������� Wepons ������ ��� �� ������ ����� ������� Wepons.Num - ��������� ���������� ��������� � �������
    EquipWeapon(CurrentWeaponIndex); // �������� EquipWeapon
}

void USTUWeaponComponent::PlayAnimMontag(UAnimMontage *Animation)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner()); // ��������� �� GetOwner()
    if (!Character)
    {
        return;
    }

    Character->PlayAnimMontage(Animation); // �������� ������� PlayAnimMontage
}

void USTUWeaponComponent::InitAnimations()
{
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify) // ���� ��� ��� Notify ��
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEnquipFinished); // ������ ��� ������� � ������ � ������� ������� AddUObject ���� ������� OnEnquipFinished
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
        checkNoEntry();
    }

    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishedNotify) // ���� �� �� ����� Notify �� ������� � ���������� �����
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
            checkNoEntry();
        }
        ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished); // ������ ��� ������� � ������ � ������� ������� AddUObject ���� ������� OnEnquipFinished
    }
}

void USTUWeaponComponent::OnEnquipFinished(USkeletalMeshComponent *MeshComp)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner()); // ��������� �� GetOwner()
    if (!Character || MeshComp != Character->GetMesh()) // �������� ��� ������ Character ���� ��������, � ��� ��� ������ ��������� �� �������� ������� ������ � MeshComponent (����� ������ �� ����������� �� ���� �������� � �� ��� ���������)
    {
        return;
    }
    EquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent *MeshComp)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner()); // ��������� �� GetOwner()
    if (!Character || MeshComp != Character->GetMesh()) // �������� ��� ������ Character ���� ��������, � ��� ��� ������
                                                        // ��������� �� �������� ������� ������ � MeshComponent (�����
                                                        // ������ �� ����������� �� ���� �������� � �� ��� ���������)
    {
        return;
    }
    ReloadAnimInProgress = false;
}

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::Reload()
{
    ChangeClip();
}

void USTUWeaponComponent::OnEmptyClip(ASTUBaseWeapon *AmmoEmptyWeapon)
{
    if (!AmmoEmptyWeapon) // ���� ��������� 0
    {
        return; // ����� �� �������
    }
    if (CurrentWeapon == AmmoEmptyWeapon) // ���� ������� ������ ��������� AmmoEmptyWeapon
    {
        ChangeClip(); // �������� �������� �����������
    }
    else // ���� ������ ������
    {
        for (const auto Weapon : Weapons) // ����������� �� ������� ������
        {
            Weapon->ChangeClip(); // � ���� �� ����� ������ ������� �� ������ ����� �������� ChangeClip ������� ������ (�������� ������������� �� �����, �� ������ ����������� �����)
        }
    }
}

void USTUWeaponComponent::ChangeClip()
{
    if (!CanReload())
    {
        return;
    }
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();
    ReloadAnimInProgress = true;
    PlayAnimMontag(CurrentReloadAnimMontage); // ������������ �������� �����������
}

bool USTUWeaponComponent::IsFiring() const
{
    return CurrentWeapon && CurrentWeapon->IsFiring(); // ���������� true ������ ���� � �������� ��������
}

bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData &UIData) const
{
    if (CurrentWeapon) // ���� ������ ����
    {
        UIData = CurrentWeapon->GetUIData(); // ���������� �������� ������ UIData
        return true;
    }
    return false;
}

bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData &AmmoData) const
{
    if (CurrentWeapon) // ���� ������ ����
    {
        AmmoData = CurrentWeapon->GetAmmoData(); // ���������� ������� �������
        return true;
    }
    return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
    for (const auto Weapon : Weapons) // ��������� �� ������� ������
    {
        if (Weapon && Weapon->IsA(WeaponType)) // ��������� ��������� Weapon �� 0 � ����������-�� ������ ��� ������
        {
            return Weapon->TryToAddAmmo(ClipsAmount); // ���� ������� ��������� , �������� ������� TryToAddAmmo(���-�� ���������)
        }
    }
    return false; // ����� �� �������
}

bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType)
{
    for (const auto Weapon : Weapons) // ��������� �� ������� ������
    {
        if (Weapon && Weapon->IsA(WeaponType)) // ��������� ��������� Weapon �� 0 � ����������-�� ������ ��� ������
        {
            return !Weapon->IsAmmoFull(); // ���� ������� ��������� , �������� ������� IsAmmoFull() - ����������� ��� ��������
        }
    }
    return false; // ����� �� �������
}

