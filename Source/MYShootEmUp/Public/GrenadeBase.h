// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeBase.generated.h"

class USoundCue;

UCLASS()
class MYSHOOTEMUP_API AGrenadeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenadeBase();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EffectRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MoveOnPathSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "VFX")
	UParticleSystem* ExplosionParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "VFX")
	USoundCue* ExplosionSound;	

	/* Positions on a parabolic paths */
	TArray<FVector> FollowPathPositions;
	
	int PathPositionIndex;	

	bool bCanMove;
	
public:

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	virtual void Explode();

	UFUNCTION(BlueprintCallable)
	void MoveToDestination(const TArray<FVector>& FollowPathPositions);

	FORCEINLINE float GetEffectRadius() const { return EffectRadius;}

};
