// Shoo Them Up Game


#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

void USTUAIWeaponComponent::StartFire()
{
	if (!CanFire()) // ���� �� �������� �� �����
	{
		return; // ����� �� �������
	}

	if (CurrentWeapon->IsAmmoEmpty()) // ��������� � ��� ���������� � ������� ������
	{
		NextWeapon(); // ���� ����������� ����, �� �������� ������� ��������� ������
	}
	else // ���� ���������� ��������
	{
		CurrentWeapon->StartFire(); // �������� ������ �������� �������� ������
	}
}

void USTUAIWeaponComponent::NextWeapon()
{
	if (!CanEquip()) // ���� �� �� ����� ������� ������
	{
		return; // ������� �� �������
	}
	int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num(); // ��������� �������� ������� ������������ �� 1 � �� ������ ������� ������, ����� NextIndex �� ����� �� ��� �������
	while (NextIndex != CurrentWeaponIndex) // ��������� ������ ������ �� ��������� �������� ������� ������, �� ����� ����������� NextIndex �� 1 �� ��� ��� ����� �� ����� ������ � �������� ���� �������. ���� NextIndex == CurrentWeaponIndex ��������� ���� (� ��� ��� ������)
	{
		if (!Weapons[NextIndex]->IsAmmoEmpty()) // ���� ������ � NextIndex ����� ���������� IsAmmoEmpty = false
		{
			break; // ��������� ���� (����� ���� ������)
		}
		NextIndex = (NextIndex + 1) % Weapons.Num(); // ���� ������ ������ NextIndex + 1 % Weapons.Num() - ����� �� ����� �� ������� �������
	}

	if (CurrentWeaponIndex != NextIndex) // ���� ������� ������ ������ �� ����� ���������� ������� ������ (�������� ��� �� ����� ������ ������)
	{
		CurrentWeaponIndex = NextIndex; // �������� ������� ������ ����������� ��������� ������ ������
		EquipWeapon(CurrentWeaponIndex); // ������ ������ � �����
	}
}
