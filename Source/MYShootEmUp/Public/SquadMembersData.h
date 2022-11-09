// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SquadMembersData.generated.h"


class AMYCharacterBase;
class ADrone;
/**
 * 
 */
UCLASS()
class MYSHOOTEMUP_API USquadMembersData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AMYCharacterBase>> SquadMembers;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADrone> Drone;	
};
