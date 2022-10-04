// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MYBTTask_ProjectileAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UMYBTTask_ProjectileAttack::UMYBTTask_ProjectileAttack() :
	MaxProjectileSpread(2.0f)
{
}

EBTNodeResult::Type UMYBTTask_ProjectileAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(("TargetActor")));
		if (!TargetActor)
		{
			return EBTNodeResult::Failed;
		}		
		
		const FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("headSocket");
		const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Yaw += FMath::RandRange(-MaxProjectileSpread,MaxProjectileSpread);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = MyPawn;		

		const AActor* Proj = GetWorld()->SpawnActor<AActor>(ProjectileClass,MuzzleLocation,MuzzleRotation,SpawnParams);

		return Proj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		
	}
	
	return EBTNodeResult::Failed;
}
