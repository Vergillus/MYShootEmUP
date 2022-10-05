// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MYAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "LootGeneratorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MYShootEmUp/HealthComponent.h"

// Sets default values
AMYAICharacter::AMYAICharacter() :
	AttackRange(150)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Comp"));
	HealthComp->OnDeath.AddDynamic(this, &AMYAICharacter::OnDeath);

	LootGenComp = CreateDefaultSubobject<ULootGeneratorComponent>(TEXT("Loot Generator"));
	

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMYAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AAIController* AICont = Cast<AAIController>(GetController()))
	{
		AICont->GetBlackboardComponent()->SetValueAsFloat("AttackRange",AttackRange);
	}
}


void AMYAICharacter::OnDeath()
{
	// stop BT
	if(const AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->GetBrainComponent()->StopLogic("Killed");
	}

	// Ragdoll
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	GetMesh()->SetSimulatePhysics(true);

	HealthComp->SetActive(false);
			
	// set Lifespan
	SetLifeSpan(2.0f);
}

void AMYAICharacter::SetTarget(AActor* NewTarget)
{
	if (AAIController* AICont = Cast<AAIController>(GetController()))
	{	
		AICont->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}
