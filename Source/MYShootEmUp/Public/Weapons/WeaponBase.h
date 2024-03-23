// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Helper.h"
#include "MYProjectileBase.h"
#include "WeaponBase.generated.h"

class UNiagaraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMagazineEmpty);

UCLASS()
class MYSHOOTEMUP_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	/* Mesh of the weapon (Root) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	EWeaponType WeaponType;
	
	/* Hit Damage of the Weapon */
	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	float WeaponDamage;

	/* Weapons fire interval */
	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	float FireRate;

	/* Range of the weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Weapon")
	float FireRange;	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Weapon")
	float SweepRadius;

	/* How many bullets can a weapon hold. We will discard weapon when MagazineCapacity reaches 0  */
	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	int32 MagazineCapacity;	

	/* Name of the muzzle socket  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Weapon")
	FName MuzzleSocketName;

	/* Name of the grip point socket */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Weapon")
	FName GripPointSocketName;

	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	UAnimMontage* FireAnimMontage;

	/* Sound to play when fire */
	UPROPERTY(EditDefaultsOnly, Category= "VFX")
	USoundBase* WeaponFireSound;		
	
	FTimerHandle CooldownTimerHandle;
	
	bool bCanFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Weapon")
	bool bIsDefaultWeapon;

	/* Cooldown timer to prevent firing too many shots. */
	virtual void CooldownTimer();

	/* Cast a ray to locate the enemy and apply WeaponDamage if found */
	virtual void LocateNHurtEnemy();

	void DecreaseMagazineCapacity();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayMuzzleFlash();

public:

	virtual void FireWeapon();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const {return WeaponMesh;}
	FORCEINLINE void SetIsDefaultWeapon(const bool Val) {bIsDefaultWeapon = Val;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EWeaponType GetWeaponType() const {return WeaponType;}

	FORCEINLINE float GetFireRange() const {return FireRange;}
	FORCEINLINE float GetSweepRadius() const {return SweepRadius;}
	
	/* Called when this weapon deemed to be discarded */
	virtual void DiscardWeapon();

	FOnMagazineEmpty OnMagazineEmpty;
};
