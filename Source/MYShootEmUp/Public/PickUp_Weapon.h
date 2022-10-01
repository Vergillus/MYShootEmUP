// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MYShootEmUp/BasePickUp.h"
#include "PickUp_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class MYSHOOTEMUP_API APickUp_Weapon : public ABasePickUp
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	TSubclassOf<AWeaponBase> WeaponToSpawn;
	
protected:
	virtual void OnPicked(AMYCharacterBase* OverlappedCharacter) override;	
};
