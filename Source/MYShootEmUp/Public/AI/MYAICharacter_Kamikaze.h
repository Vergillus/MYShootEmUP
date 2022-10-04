// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/MYAICharacter.h"
#include "MYAICharacter_Kamikaze.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class MYSHOOTEMUP_API AMYAICharacter_Kamikaze : public AMYAICharacter
{
	GENERATED_BODY()

public:
	AMYAICharacter_Kamikaze();

	virtual void BeginPlay() override;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Explosion")
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Explosion")
	float ExplosionDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Explosion")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Explosion | VFX")
	UParticleSystem* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Explosion | VFX")
	USoundBase* ExplosionSound;

	bool bIsInitiatedKamikaze;

	virtual void OnDeath() override;

public:
	UFUNCTION(BlueprintCallable)
	void Explode();	
	
};
