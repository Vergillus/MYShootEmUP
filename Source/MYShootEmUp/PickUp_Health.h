// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickUp.h"
#include "PickUp_Health.generated.h"

/**
 * 
 */
UCLASS()
class MYSHOOTEMUP_API APickUp_Health : public ABasePickUp
{
	GENERATED_BODY()

public:
	APickUp_Health();

	virtual void BeginPlay() override;
	
private:

	/* If true then we randomly pick a heal amount on begin play */
	UPROPERTY(EditDefaultsOnly, Category="Health", meta = (AllowPrivateAccess = "true"))
	bool bRandomHealAmount;
	
	UPROPERTY(EditDefaultsOnly, Category="Health", meta = (AllowPrivateAccess = "true"))
	int32 HealAmount;

	virtual void OnPicked(AMYCharacterBase* OverlappedCharacter) override;
};
