// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "MYShootEmUpGameMode.generated.h"

class ABasePickUp;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

UCLASS(minimalapi)
class AMYShootEmUpGameMode : public AGameModeBase
{
	GENERATED_BODY()	

public:
	AMYShootEmUpGameMode();

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ABasePickUp>> PickUpList;

	UPROPERTY(EditDefaultsOnly, Category= "AI", meta=(AllowPrivateAccess = "true"))
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category= "AI", meta=(AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AActor>> BotsToSpawn; 
	
	UPROPERTY(EditDefaultsOnly, Category= "AI")
	float SpawnTimerInterval;

	FTimerHandle SpawnBotTimerHandle;

	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UPROPERTY(VisibleAnywhere)
	int KillCount;

public:

	UFUNCTION(BlueprintCallable)
	TSubclassOf<ABasePickUp> GetRandomPickUp();

	FORCEINLINE int GetKillCount() const {return KillCount;}
	FORCEINLINE void IncreaseKillCount() { KillCount++ ;}
};



