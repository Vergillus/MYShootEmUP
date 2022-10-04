// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MYBTTask_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class MYSHOOTEMUP_API UMYBTTask_ProjectileAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMYBTTask_ProjectileAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:

	UPROPERTY(EditAnywhere, Category= "AI")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category= "AI")
	float MaxProjectileSpread;

	
};
