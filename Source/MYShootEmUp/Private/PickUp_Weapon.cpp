// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp_Weapon.h"

#include "MYShootEmUp/MYCharacterBase.h"

void APickUp_Weapon::OnPicked(AMYCharacterBase* OverlappedCharacter)
{
	OverlappedCharacter->EquipWeapon(WeaponToSpawn);
	Super::OnPicked(OverlappedCharacter);
}
