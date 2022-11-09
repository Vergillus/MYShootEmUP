// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MYShootEmUp/HealthComponent.h"

#include "NiagaraSystem.h"

// Sets default values
AWeaponBase::AWeaponBase() :
	WeaponDamage(10),
	FireRate(0.5f),
	FireRange(1000),
	SweepRadius(10.0f),
	MagazineCapacity(100),
	MuzzleSocketName("MuzzleLocation"),
	GripPointSocketName("GripPoint"),
	bCanFire(true)
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

	PlayMuzzleFlash();

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

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(SweepRadius);
	
	if(GetWorld()->SweepSingleByObjectType(HitResult,TraceStart,TraceEnd, MuzzleSocketTransform.GetRotation(),ECollisionChannel::ECC_Pawn, CollisionShape,QueryParams))
	{
		if(auto HealthComp = Cast<UHealthComponent>(HitResult.GetActor()->GetComponentByClass(UHealthComponent::StaticClass())))
		{
			HealthComp->DamageHealth(GetOwner(), WeaponDamage);
		}
	}

}

void AWeaponBase::DiscardWeapon()
{
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMesh->SetCollisionProfileName(FName("Weapon"));	
	WeaponMesh->SetSimulatePhysics(true);
	const FVector Impulse = (GetActorForwardVector() + GetActorUpVector()) * 950.0f;	
	WeaponMesh->AddImpulse(Impulse);	

	SetLifeSpan(1.5f);
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
