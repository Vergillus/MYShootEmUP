// Fill out your copyright notice in the Description page of Project Settings.


#include "MYProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MYShootEmUp/HealthComponent.h"
#include "Niagara/Public/NiagaraComponentPool.h"

// Sets default values
AMYProjectileBase::AMYProjectileBase() :
	DamageAmount(10)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	RootComponent = SphereCollision;

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AMYProjectileBase::OnActorOverlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Comp"));
	ProjectileMovementComponent->InitialSpeed = 2000.0f;
}

// Called when the game starts or when spawned
void AMYProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMYProjectileBase::ApplyDamage(AActor* DamageReceiver)
{
	if (const auto HealthComp = Cast<UHealthComponent>(DamageReceiver->GetComponentByClass(UHealthComponent::StaticClass())))
	{
		HealthComp->DamageHealth(GetInstigator(),DamageAmount);
	}
}

void AMYProjectileBase::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetInstigator()) return;

	ApplyDamage(OtherActor);

	Destroy();
	
}
