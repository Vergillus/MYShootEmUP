// Fill out your copyright notice in the Description page of Project Settings.


#include "MYShootEmUp/Public/GrenadeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"

// Sets default values
AGrenadeBase::AGrenadeBase() :
	EffectRadius(500),
	Damage(25),
	MoveOnPathSpeed(1.0f),
	bCanMove(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = Mesh;	
}

void AGrenadeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bCanMove)
	{	
		const FVector EndPos = FollowPathPositions[PathIndex];

		const float SegmentSpeed = MoveOnPathSpeed * FollowPathPositions.Num();

		if (FVector::Distance(GetActorLocation(), EndPos) > 0.1f)
		{
			const FVector NewPos = FMath::VInterpTo(GetActorLocation(),EndPos,DeltaSeconds, SegmentSpeed);
			SetActorLocation(NewPos);			
		}
		else
		{
			PathIndex++;
			SegmentT = 0.0f;

			if (PathIndex > FollowPathPositions.Num() - 1)
			{
				Explode();
				bCanMove = false;
				PrimaryActorTick.bCanEverTick = false;
				Destroy();
			}	
		}		
	}
}

void AGrenadeBase::Explode()
{
	if(ExplosionSound)
	{
		UGameplayStatics::PlaySound2D(this,ExplosionSound);		
	}
}

void AGrenadeBase::MoveToDestination(TArray<FVector> PathPositions)
{
	FollowPathPositions = PathPositions;
	PathIndex = 1;
	bCanMove = true;
}

