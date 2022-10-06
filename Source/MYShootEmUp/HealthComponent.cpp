// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent():
	MaxHealth(100),
	CurrentHealth(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}


void UHealthComponent::DamageHealth(AActor* Instigator,const float Damage)
{
	if(Damage <= 0 || CurrentHealth <= 0) return;

	CurrentHealth -= Damage;
	CurrentHealth = FMath::Max(CurrentHealth,0);

	OnHealthChanged.Broadcast(Instigator,MaxHealth,CurrentHealth);
	
	if (CurrentHealth <= 0)
	{
		OnDeath.Broadcast();		
	}
}

void UHealthComponent::Heal(const float Amount)
{
	if (Amount <= 0) return;

	CurrentHealth = FMath::Min(CurrentHealth + Amount, MaxHealth);

	UE_LOG(LogTemp,Warning,TEXT("Healed : Heal Amount %f Health %f"), Amount, CurrentHealth);

	OnHealthChanged.Broadcast(nullptr,MaxHealth,CurrentHealth);
}


