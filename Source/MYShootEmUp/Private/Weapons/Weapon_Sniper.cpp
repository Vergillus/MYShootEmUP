// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon_Sniper.h"

#include "MYShootEmUp/HealthComponent.h"

AWeapon_Sniper::AWeapon_Sniper()	
{
	WeaponType = EWeaponType::EWT_Sniper;
}

void AWeapon_Sniper::LocateNHurtEnemy()
{
	TArray<FHitResult> HitResults;

	const FTransform MuzzleSocketTransform{WeaponMesh->GetSocketTransform(MuzzleSocketName)};
	
	const FVector SweepStart{MuzzleSocketTransform.GetLocation()};
	const FVector SweepEnd{SweepStart + MuzzleSocketTransform.GetRotation().GetForwardVector() * FireRange};	

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);	

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(SweepRadius);	

	// Find all actors in range
	if(GetWorld()->SweepMultiByObjectType(HitResults,SweepStart,SweepEnd,GetActorRotation().Quaternion(),ObjectQueryParams,CollisionShape, QueryParams))
	{
		// Apply damage
		for (auto HitResult : HitResults)
		{
			if(auto HealthComp = Cast<UHealthComponent>(HitResult.GetActor()->GetComponentByClass(UHealthComponent::StaticClass())))
			{
				HealthComp->DamageHealth(GetOwner(), WeaponDamage);
			}
		}
	}
}
