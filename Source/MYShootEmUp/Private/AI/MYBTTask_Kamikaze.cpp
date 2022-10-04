// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MYBTTask_Kamikaze.h"

#include "AIController.h"
#include "AI/MYAICharacter_Kamikaze.h"

EBTNodeResult::Type UMYBTTask_Kamikaze::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		AMYAICharacter_Kamikaze* MyPawn = Cast<AMYAICharacter_Kamikaze>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}		

		MyPawn->Explode();
		return EBTNodeResult::Succeeded;
	}	
	return EBTNodeResult::Failed;
}
