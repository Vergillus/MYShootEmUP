// Copyright Epic Games, Inc. All Rights Reserved.

#include "MYShootEmUpGameMode.h"
#include "MYShootEmUpPlayerController.h"
#include "MYShootEmUpCharacter.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "UObject/ConstructorHelpers.h"

AMYShootEmUpGameMode::AMYShootEmUpGameMode() :
	SpawnTimerInterval(2.0f),
	KillCount(0),
	bCanSpawnBots(true)
{
	// use our custom PlayerController class
	PlayerControllerClass = AMYShootEmUpPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void AMYShootEmUpGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(bCanSpawnBots)
	{
		GetWorldTimerManager().SetTimer(SpawnBotTimerHandle, this, &AMYShootEmUpGameMode::SpawnBotTimerElapsed, SpawnTimerInterval,true);		
	}
}

void AMYShootEmUpGameMode::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery,this, EEnvQueryRunMode::RandomBest5Pct,nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&AMYShootEmUpGameMode::OnSpawnBotQueryCompleted);
	}
}

void AMYShootEmUpGameMode::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (!Locations.IsEmpty())
	{
		for (auto Location : Locations)
		{
			const int RndIndex = FMath::RandRange(0, BotsToSpawn.Num() - 1);
			GetWorld()->SpawnActor<AActor>(BotsToSpawn[RndIndex], Location, FRotator::ZeroRotator);
		}
	}
}

TSubclassOf<ABasePickUp> AMYShootEmUpGameMode::GetRandomPickUp()
{
	const int32 RndIndex = FMath::RandRange(0, PickUpList.Num() - 1);

	return PickUpList[RndIndex];
}

void AMYShootEmUpGameMode::GameOver()
{
	GetWorldTimerManager().ClearTimer(SpawnBotTimerHandle);
}
