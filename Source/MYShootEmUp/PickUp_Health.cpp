// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp_Health.h"

#include "HealthComponent.h"

APickUp_Health::APickUp_Health() :
	bRandomHealAmount(false),
	HealAmount(20)
{
	
}

void APickUp_Health::BeginPlay()
{
	Super::BeginPlay();

	if (bRandomHealAmount)
	{
		HealAmount = FMath::RandRange(50,100);
	}
}

void APickUp_Health::OnSphereColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UHealthComponent* OverlappedHealthComponent = Cast<UHealthComponent>(OtherActor->GetComponentByClass(UHealthComponent::StaticClass())))
	{
		OverlappedHealthComponent->Heal(HealAmount);
		OnPicked();
	}
}

void APickUp_Health::OnPicked()
{
	Super::OnPicked();

	Destroy();
}
