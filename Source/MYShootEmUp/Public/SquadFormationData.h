// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Helper.h"
#include "SquadFormationData.generated.h"

/**
 * 
 */
UCLASS()
class MYSHOOTEMUP_API USquadFormationData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category= "Formation")
	ESquadFormationType FormationType;

	UPROPERTY(EditDefaultsOnly, Category= "Formation")
	TArray<FVector> FormationPositions;

	UPROPERTY(EditDefaultsOnly, Category= "Formation")
	TArray<FRotator> FormationRotations;
};
