// Copyright Epic Games, Inc. All Rights Reserved.

#include "MYShootEmUpGameMode.h"
#include "MYShootEmUpPlayerController.h"
#include "MYShootEmUpCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMYShootEmUpGameMode::AMYShootEmUpGameMode()
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

TSubclassOf<ABasePickUp> AMYShootEmUpGameMode::GetRandomPickUp()
{
	const int32 RndIndex = FMath::RandRange(0, PickUpList.Num() - 1);

	return PickUpList[RndIndex];
}
