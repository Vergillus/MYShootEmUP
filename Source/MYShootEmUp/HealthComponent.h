// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged,AActor*, Instigator, float, MaxHealth, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSHOOTEMUP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();	

protected:	

	UPROPERTY(EditDefaultsOnly, Category= "Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category= "Health")
	float CurrentHealth;	

public:

	UFUNCTION(BlueprintCallable)
	void DamageHealth(AActor* Instigator, const float Damage);
	
	void Heal(const float Amount);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;
		
};
