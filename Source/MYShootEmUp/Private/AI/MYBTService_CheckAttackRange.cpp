// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MYBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UMYBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check Distance between AI pawn and target actor
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		if(const AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor")))
		{
			const AAIController* MyController = OwnerComp.GetAIOwner();			
			if (ensure(MyController))
			{
				const APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					const float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());					

					const float AttackDistance = BlackboardComp->GetValueAsFloat(AttackRangeKey.SelectedKeyName);
					const bool bWithingRange = DistanceTo < AttackDistance;
//
					bool bHasLineOfSight = false;
					if(bWithingRange)
					{
						bHasLineOfSight = MyController->LineOfSightTo(TargetActor);						
					}

					BlackboardComp->SetValueAsBool(CanAttackKey.SelectedKeyName, (bWithingRange && bHasLineOfSight));
				}
			}
		}
	}
}
