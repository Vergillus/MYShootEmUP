// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

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

	/* Hit Damage of the Weapon */
	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	float WeaponDamage;

	/* Weapons fire interval */
	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	float FireRate;

	/* Range of the weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Weapon")
	float FireRange;

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

	/* Particles system that shows the range of the weapon */
	UPROPERTY(EditDefaultsOnly, Category= "VFX")
	UParticleSystem* LaserSightParticleSystem;
	
	FTimerHandle CooldownTimerHandle;
	
	bool bCanFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Weapon")
	bool bIsDefaultWeapon;

	/* Cooldown timer to prevent firing too many shots. */
	virtual void CooldownTimer();

	/* Cast a ray to locate the enemy and apply WeaponDamage if found */
	virtual void LocateNHurtEnemy();

	/* Called when this weapon deemed to be discarded */
	virtual void DiscardWeapon();

	void DecreaseMagazineCapacity();

public:

	virtual void FireWeapon();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const {return WeaponMesh;}
	FORCEINLINE void SetIsDefaultWeapon(const bool Val) {bIsDefaultWeapon = Val;}

	FOnMagazineEmpty OnMagazineEmpty;
};
