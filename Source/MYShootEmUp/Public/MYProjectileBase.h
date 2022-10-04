// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MYProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;

UCLASS()
class MYSHOOTEMUP_API AMYProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMYProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category= "Projectile")
	USphereComponent* SphereCollision;

	UPROPERTY(EditDefaultsOnly, Category= "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;

	// UPROPERTY(EditDefaultsOnly, Category= "Projectile")
	// UNiagaraComponent* ProjectileParticle;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	float DamageAmount;

	virtual void ApplyDamage(AActor* DamageReceiver);

	UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
