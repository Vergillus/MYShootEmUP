// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MYShootEmUp/BasePickUp.h"
#include "PickUp_Grenade.generated.h"

/**
 * 
 */
UCLASS()
class MYSHOOTEMUP_API APickUp_Grenade : public ABasePickUp
{
	GENERATED_BODY()

protected:
	virtual void OnPicked(AMYCharacterBase* OverlappedCharacter) override;
};
