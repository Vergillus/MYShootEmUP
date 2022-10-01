// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"

#include "MYShootEmUp/HealthComponent.h"

// Sets default values
AWeaponBase::AWeaponBase() :
	WeaponDamage(10),
	FireRate(0.5f),
	FireRange(1000),
	MagazineCapacity(100),
	bCanFire(true),
	GripPointSocketName("GripPoint")
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBase::FireWeapon()
{
	if(!bCanFire) return;

	bCanFire =false;

	LocateNHurtEnemy();

	DecreaseMagazineCapacity();
	
	GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AWeaponBase::CooldownTimer, FireRate,false);
}

void AWeaponBase::CooldownTimer()
{
	bCanFire = true;
}

void AWeaponBase::LocateNHurtEnemy()
{
	FHitResult HitResult;

	const FTransform MuzzleSocketTransform{WeaponMesh->GetSocketTransform(MuzzleSocketName)};
	
	const FVector TraceStart{MuzzleSocketTransform.GetLocation()};
	const FVector TraceEnd{TraceStart + MuzzleSocketTransform.GetRotation().GetForwardVector() * FireRange};

	DrawDebugLine(GetWorld(),TraceStart,TraceEnd,FColor::Red,false, 2.0f,0,2.5f);
	

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if(GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd,ECollisionChannel::ECC_Visibility,QueryParams))
	{
		if(auto HealthComp = Cast<UHealthComponent>(HitResult.GetActor()->GetComponentByClass(UHealthComponent::StaticClass())))
		{
			HealthComp->DamageHealth(GetOwner(), WeaponDamage);
		}
	}

}

void AWeaponBase::DiscardWeapon()
{
	
}

void AWeaponBase::DecreaseMagazineCapacity()
{
	if (!bIsDefaultWeapon)
	{
		MagazineCapacity--;
	}

	if (MagazineCapacity <= 0)
	{
		OnMagazineEmpty.Broadcast();
	}
}
