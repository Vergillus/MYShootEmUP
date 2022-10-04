// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MYAICharacter.generated.h"

class UHealthComponent;

UCLASS()
class MYSHOOTEMUP_API AMYAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMYAICharacter();

	virtual void BeginPlay() override;

protected:	

	UPROPERTY(EditDefaultsOnly, Category= "Components")
	UHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Attack")
	float AttackRange;

	UFUNCTION(BlueprintCallable)
	virtual void OnDeath();

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* NewTarget);
};
