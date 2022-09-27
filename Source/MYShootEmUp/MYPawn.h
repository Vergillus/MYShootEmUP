// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MYPawn.generated.h"

class UFloatingPawnMovement;
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class MYSHOOTEMUP_API AMYPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMYPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category= "Movement")
	UFloatingPawnMovement* PawnMovementComp;

	UPROPERTY(EditAnywhere, Category= "Collision")
	UCapsuleComponent* CapsuleCollider;

	UPROPERTY(EditAnywhere, Category= "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category= "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category= "Movement")
	USceneComponent* RotParentByMouse;

	UPROPERTY(EditDefaultsOnly, Category= "Movement")
	USceneComponent* SquadParent;

	UPROPERTY(EditDefaultsOnly)
	TArray<UStaticMeshComponent*> SquadMemberPositions;

#pragma region Movement Related Functions

	UFUNCTION(BlueprintCallable, Category= "Pawn Movement")
	void MoveForwardBack(float AxisVal);

	UFUNCTION(BlueprintCallable, Category= "Pawn Movement")
	void MoveLeftRight(float AxisVal);

	UFUNCTION(BlueprintCallable, Category= "Pawn Movement")
	void RotateSquadMembers();

	UFUNCTION()
	void RotateSquadInTime(const float Duration, const FRotator CurrentRot, const FRotator DesiredRot);

#pragma endregion

	FTimerHandle CharacterRotationTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category= "Character Rotation")
	float CharacterRotationDuration;
	float CharacterRotLerpVal;
	bool bCanRotateCharacters;

	bool bCanThrowGrenade;
	FVector2d CachedMousePos;

	void GrenadeThrowStart();
	void GrenadeThrowEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input //
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
