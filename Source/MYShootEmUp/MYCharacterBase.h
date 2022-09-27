// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MYCharacterBase.generated.h"

class UHealthComponent;

UCLASS()
class MYSHOOTEMUP_API AMYCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMYCharacterBase();

private:
	UPROPERTY(EditDefaultsOnly, Category= "Components", meta=(AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDeathHandler();

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Fire();	

};
