// Fill out your copyright notice in the Description page of Project Settings.


#include "LootGeneratorComponent.h"

#include "Kismet/GameplayStatics.h"
#include "MYShootEmUp/BasePickUp.h"
#include "MYShootEmUp/HealthComponent.h"
#include "MYShootEmUp/MYShootEmUpGameMode.h"

// Sets default values for this component's properties
ULootGeneratorComponent::ULootGeneratorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULootGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();

	if(const auto HealthComp = Cast<UHealthComponent>(GetOwner()->GetComponentByClass(UHealthComponent::StaticClass())))
	{
		HealthComp->OnDeath.AddDynamic(this, &ULootGeneratorComponent::GenerateLootOnDead);
	}
	
}

void ULootGeneratorComponent::GenerateLootOnDead()
{
	if(const auto GM = Cast<AMYShootEmUpGameMode>(UGameplayStatics::GetGameMode(GetOwner())))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (const auto PickUpToSpawn = GM->GetRandomPickUp())
		{
			
			 if(GetWorld()->SpawnActor<ABasePickUp>(PickUpToSpawn ,GetOwner()->GetActorLocation(),FRotator::ZeroRotator,SpawnParameters))
			 {
			 	UE_LOG(LogTemp,Warning,TEXT("LOOOOOOT"));
			 }
		}		
	}
}



