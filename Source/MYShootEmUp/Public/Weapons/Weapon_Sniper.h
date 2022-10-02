// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "Weapon_Sniper.generated.h"

/**
 * 
 */
UCLASS()
class MYSHOOTEMUP_API AWeapon_Sniper : public AWeaponBase
{
	GENERATED_BODY()

public:
	AWeapon_Sniper();
	
	
protected:
	
	virtual void LocateNHurtEnemy() override;
	
};
