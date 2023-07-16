// Shoo Them Up Game


#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

void USTUAIWeaponComponent::StartFire()
{
	if (!CanFire()) // если мы стрелять не можем
	{
		return; // выход из функции
	}

	if (CurrentWeapon->IsAmmoEmpty()) // существую у нас боеприпасы в текущем оружии
	{
		NextWeapon(); // если боеприпасов нету, то вызываем функцию следующее оружие
	}
	else // если боеприпасы остались
	{
		CurrentWeapon->StartFire(); // вызываем начало стрельбы текущего оружия
	}
}

void USTUAIWeaponComponent::NextWeapon()
{
	if (!CanEquip()) // если мы не можем сменить оружие
	{
		return; // выходим из фукнции
	}
	int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num(); // равняется текущему индексу увеличенному на 1 и по модулю размера масива, чтобы NextIndex не вышел за его пределы
	while (NextIndex != CurrentWeaponIndex) // Следующий индекс оружия не равняется Текущему индексу оружия, мы будем увеличивать NextIndex на 1 до тех пор когда не найдём оружие у которого есть патроны. Если NextIndex == CurrentWeaponIndex завершаем цикл (у нас нет оружия)
	{
		if (!Weapons[NextIndex]->IsAmmoEmpty()) // если оружие с NextIndex имеет боеприпасы IsAmmoEmpty = false
		{
			break; // завершаем цикл (нашли наше оружие)
		}
		NextIndex = (NextIndex + 1) % Weapons.Num(); // если оружие пустое NextIndex + 1 % Weapons.Num() - чтобы не выйти за границы массива
	}

	if (CurrentWeaponIndex != NextIndex) // если текущий индекс оружия не равен следующему индексу оружия (означает что мы нашли другое оружие)
	{
		CurrentWeaponIndex = NextIndex; // текущему индексу оружия присваиваем следующий индекс оружия
		EquipWeapon(CurrentWeaponIndex); // меняем оружие в руках
	}
}
