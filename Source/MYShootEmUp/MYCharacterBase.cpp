// Fill out your copyright notice in the Description page of Project Settings.


#include "MYCharacterBase.h"

#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMYCharacterBase::AMYCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Comp"));
	HealthComponent->OnDeath.AddDynamic(this,&AMYCharacterBase::OnDeathHandler);
}

// Called when the game starts or when spawned
void AMYCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMYCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMYCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMYCharacterBase::Fire()
{
	
}

void AMYCharacterBase::OnDeathHandler()
{
	// Clear parent
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,false));
	
	// Play Death montage
	if (DeathMontage)
	{
		
	}

	// Notify squad
}

