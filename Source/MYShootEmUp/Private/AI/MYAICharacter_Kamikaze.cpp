// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MYAICharacter_Kamikaze.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MYShootEmUp/HealthComponent.h"

AMYAICharacter_Kamikaze::AMYAICharacter_Kamikaze() :
	ExplosionRadius(50),
	ExplosionDamage(50),	
	bIsInitiatedKamikaze(false)
{
}

void AMYAICharacter_Kamikaze::OnDeath()
{
	if(!bIsInitiatedKamikaze)
	{
		Explode();		
	}
	
	Super::OnDeath();

	const FVector Impulse{-GetActorForwardVector() + GetActorUpVector() * 15000.0f};
	GetMesh()->AddImpulse(Impulse, NAME_None, true);
}

void AMYAICharacter_Kamikaze::Explode()
{
	if (ExplosionParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle,GetActorLocation());
	}

	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSound2D(this,ExplosionSound);
	}
	

	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic),UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)};
	const TArray<AActor*> ActorsToIgnore{this};
	TArray<AActor*> OverlappedActors;
	
	UKismetSystemLibrary::SphereOverlapActors(this,GetActorLocation(),ExplosionRadius,ObjectTypes,nullptr,ActorsToIgnore,OverlappedActors);

	for (const auto Actor : OverlappedActors)
	{
		if (const auto HealthComponent = Cast<UHealthComponent>(Actor->GetComponentByClass(UHealthComponent::StaticClass())))
		{
			HealthComponent->DamageHealth(this, ExplosionDamage);
		}
	}
	
	if(HealthComp->GetCurrentHealth() != 0)
	{
		bIsInitiatedKamikaze = true;

		HealthComp->DamageHealth(nullptr, HealthComp->GetMaxHealth());
		HealthComp->OnDeath.RemoveDynamic(this, &AMYAICharacter_Kamikaze::OnDeath);
		
		//OnDeath();
	}
}
