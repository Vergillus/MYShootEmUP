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

void APickUp_Health::OnPicked(AMYCharacterBase* OverlappedCharacter)
{
	if (UHealthComponent* OverlappedHealthComponent = Cast<UHealthComponent>(OverlappedCharacter->GetComponentByClass(UHealthComponent::StaticClass())))
	{
		OverlappedHealthComponent->Heal(HealAmount);		
	}
	
	Super::OnPicked(OverlappedCharacter);	
}


