// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MYAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AMYAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviourTree in your AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);		
	}

	if(APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this,0))
	{
		//GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor",MyPawn);
	}
}
