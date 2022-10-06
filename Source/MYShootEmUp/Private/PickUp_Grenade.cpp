// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp_Grenade.h"

#include "Kismet/GameplayStatics.h"
#include "MYShootEmUp/MYPawn.h"

void APickUp_Grenade::OnPicked(AMYCharacterBase* OverlappedCharacter)
{
	if(AMYPawn* PlayerPawn = Cast<AMYPawn>(UGameplayStatics::GetPlayerPawn(this,0)))
	{
		PlayerPawn->GrenadePickedUp();
	}
	
	Super::OnPicked(OverlappedCharacter);	
}
