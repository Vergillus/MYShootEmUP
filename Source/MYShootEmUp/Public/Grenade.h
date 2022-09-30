// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrenadeBase.h"
#include "Grenade.generated.h"

/**
 * 
 */
UCLASS()
class MYSHOOTEMUP_API AGrenade : public AGrenadeBase
{
	GENERATED_BODY()

public:

	virtual void Explode() override;
};
