// Fill out your copyright notice in the Description page of Project Settings.


#include "MYShootEmUp/Public/Grenade.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MYShootEmUp/HealthComponent.h"

void AGrenade::Explode()
{
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic)};
	const TArray<AActor*> ActorsToIgnore{GetOwner()};
	TArray<AActor*> OverlappedActors;
	
	UKismetSystemLibrary::SphereOverlapActors(this,GetActorLocation(),EffectRadius,ObjectTypes, nullptr,ActorsToIgnore,OverlappedActors);

	for (const auto Actor : OverlappedActors)
	{
		if(UHealthComponent* HealthComp = Cast<UHealthComponent>(Actor->GetComponentByClass(UHealthComponent::StaticClass())))
		{
			HealthComp->DamageHealth(this,Damage);
		}
	}

	if(ExplosionParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,ExplosionParticle,GetActorLocation(),FRotator::ZeroRotator,FVector::OneVector * 2.0f);		
	}
	Super::Explode();
}
