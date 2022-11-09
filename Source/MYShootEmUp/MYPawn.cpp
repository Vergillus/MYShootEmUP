// Fill out your copyright notice in the Description page of Project Settings.


#include "MYPawn.h"

#include "MYCharacterBase.h"
#include "MYShootEmUpGameMode.h"
#include "SquadFormationData.h"
#include "SquadMembersData.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Drone.h"
#include "Public/GrenadeBase.h" 

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMYPawn::AMYPawn() :
	CharacterRotationDuration(0.25f),
	bCanRotateCharacters(true),
	CurrentLeaderIndex(-1),
	bCanThrowGrenade(false),
	GrenadeThrowHeight(2000),	
	SpringArmMovementSpeed(25),
	MaxGrenadeThrowDistance(1500),
	MaxGrenadeCount(5),
	CurrentGrenadeCount(MaxGrenadeCount),
	bCanFire(false),
	CurrentFormationType(ESquadFormationType::ESF_BackToBack)

{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	RootComponent = CapsuleCollider;

	CapsuleCollider->SetCapsuleHalfHeight(96.0f);
	CapsuleCollider->SetCapsuleRadius(93.0f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1500.0f;
	SpringArm->SetRelativeRotation(FRotator(-50,0,0));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArm);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Crosshair Decal"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->DecalSize = FVector{50,50,20};
	DecalComponent->SetRelativeLocation(FVector(0,0,-96));

	RotParentByMouse = CreateDefaultSubobject<USceneComponent>(TEXT("Rotate Squad by Mouse"));
	RotParentByMouse->SetupAttachment(RootComponent);

	SquadParent = CreateDefaultSubobject<USceneComponent>(TEXT("Squad Parent"));
	SquadParent->SetupAttachment(RotParentByMouse);	

	// constexpr int SquadMemberCnt = 5;
	//
	// FString SocketName{};
 //    for (int i = 0; i < SquadMemberCnt; ++i)
 //    {
 //    	SocketName = "Pos_" + FString::FromInt(i + 1);
 //        if (UStaticMeshComponent* SceneComp = CreateDefaultSubobject<UStaticMeshComponent>(FName(*SocketName)))
 //        {
	//         SceneComp->SetupAttachment(SquadParent);
 //        	SceneComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
 //        	SquadMemberPositions.Add(SceneComp);
 //        }
 //    }	

    // for (int i = 0; i < SquadMemberCnt; ++i)
    // {
    // 	SocketName = "Character_" + FString::FromInt(i);
	   //  if (UChildActorComponent* ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(FName(*SocketName)))
	   //  {
	   //  	ChildActorComponent->SetupAttachment(SquadMemberPositions[i]);
	   //  	ChildActorComponent->SetUsingAbsoluteScale(true);	 
	   //  	ChildActorComponent->SetRelativeLocation(FVector::UpVector * 90);
	   //  	ChildActorComponent->CreateChildActor();    	
    //
	   //  	SquadMembers.Add(ChildActorComponent);
	   //  }
    // }

	PawnMovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Comp"));
}

// Called when the game starts or when spawned
void AMYPawn::BeginPlay()
{
	Super::BeginPlay();

	InitializeSquad();
	
	
	AliveMembersCount = SquadMembers.Num();
	
	for (const auto SquadMember : SquadMembers)
	{
		if(SquadMember)
		{
			SquadMember->SubscribeToLeaderChange(OnLeaderChanged);			
		}
	}	
	
	ChangeLeader();
	OnGrenadeChanged.Broadcast(CurrentGrenadeCount, MaxGrenadeCount);
	
	if(GrenadeToSpawn)
	{
		if(const auto Grenade = Cast<AGrenadeBase>(GrenadeToSpawn->GetDefaultObject()))
		{
			GrenadeEffectRadius = Grenade->GetEffectRadius();
		}	
	}	

}
// Called every frame
void AMYPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	FVector MouseWorldPos;
	FVector MouseWorldDir;
	UGameplayStatics::GetPlayerController(this,0)->DeprojectMousePositionToWorld(MouseWorldPos,MouseWorldDir);

	const FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0)->GetCameraLocation();

	FVector LinePlaneIntersectionPoint;
	float T;
	if(UKismetMathLibrary::LinePlaneIntersection_OriginNormal(CameraLocation, MouseWorldDir * 10000, FVector::Zero(),FVector::UpVector, T,LinePlaneIntersectionPoint))
	{
		//DrawDebugSphere(GetWorld(),LinePlaneIntersectionPoint, 25.0f, 6, FColor::Cyan);
		FVector RotateDirection = (LinePlaneIntersectionPoint - GetActorLocation()).GetSafeNormal();
		RotateDirection.Z = 0.0f;

		if(!bCanThrowGrenade)
			RotParentByMouse->SetRelativeRotation(UKismetMathLibrary::MakeRotFromX(RotateDirection)); 
	}

	if (DecalComponent)
	{
		DecalComponent->SetWorldLocation(LinePlaneIntersectionPoint);
	}

	if (bCanThrowGrenade)
	{
		DrawDebugCircle(GetWorld(),GetActorLocation(),MaxGrenadeThrowDistance,32,FColor::Blue,false,-1,0,2,FVector::ForwardVector,FVector::RightVector);
//		DrawDebugSphere(GetWorld(),SpringArm->GetRelativeLocation(), 15.0f, 6, FColor::Magenta);

		// FVector SpringArmNewPos = FMath::Lerp(SpringArm->GetRelativeLocation(), LinePlaneIntersectionPoint, DeltaTime * 0.5f);
		// SpringArmNewPos = SpringArmNewPos.GetClampedToSize(0,1500);
		// SpringArm->SetRelativeLocation(SpringArmNewPos);		

		FVector EndPos = SpringArm->GetComponentLocation();
		EndPos.Z = 0.0f;
		VisualizeGrenadeTrajectory(GetActorLocation(), EndPos);
	}

	if (bCanFire)
	{
		for (const auto SquadMember : SquadMembers)
		{
			if(SquadMember)
			{				
				SquadMember->Fire();
			}
		}
	}
	//
}

// Called to bind functionality to input
void AMYPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveFB",this, &AMYPawn::MoveForwardBack);
	PlayerInputComponent->BindAxis("MoveLR",this, &AMYPawn::MoveLeftRight);

	PlayerInputComponent->BindAction("RotateCharacter", IE_Pressed, this,&AMYPawn::RotateSquadMembers);
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this,&AMYPawn::GrenadeThrowStart);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMYPawn::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMYPawn::EndFire);

	PlayerInputComponent->BindAction("ChangeSquadFormation", IE_Pressed, this, &AMYPawn::ChangeFormation);

}

#pragma region Movement Functions

void AMYPawn::MoveForwardBack(float AxisVal)
{
	FVector CameraForward = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0)->GetActorForwardVector();
	CameraForward.Z = 0;
	
	if(bCanThrowGrenade)
	{
		FVector NewLocation = SpringArm->GetRelativeLocation() + CameraForward * AxisVal * SpringArmMovementSpeed;
		NewLocation = NewLocation.GetClampedToSize(0,MaxGrenadeThrowDistance);
		SpringArm->SetRelativeLocation(NewLocation);		
	}
	else
	{
		PawnMovementComp->AddInputVector(CameraForward * AxisVal);

		for (const auto Member : SquadMembers)
		{
			if(Member)
			{
				Member->SetVelocity(PawnMovementComp->Velocity);
			}
		}
	}
	
}

void AMYPawn::MoveLeftRight(float AxisVal)
{
	const FVector CameraRight = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0)->GetActorRightVector();	
	if(bCanThrowGrenade)
	{
		FVector NewLocation = SpringArm->GetRelativeLocation() + CameraRight * AxisVal * SpringArmMovementSpeed;
		NewLocation = NewLocation.GetClampedToSize(0,MaxGrenadeThrowDistance);
		SpringArm->SetRelativeLocation(NewLocation);		
	}
	else
	{
		PawnMovementComp->AddInputVector(CameraRight * AxisVal);		
	}
	
}

void AMYPawn::RotateSquadMembers()
{
	if(CurrentFormationType != ESquadFormationType::ESF_BackToBack) return;
	if(!bCanRotateCharacters || bCanThrowGrenade) return;

	bCanRotateCharacters = false;

	CharacterRotLerpVal = 0.0f;

	const FRotator CurrentRot = SquadParent->GetRelativeRotation();
	const FRotator DesiredRot = UKismetMathLibrary::ComposeRotators(CurrentRot,FRotator(0,90,0));
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this,FName("RotateSquadInTime"),CharacterRotationDuration, CurrentRot, DesiredRot);

	GetWorldTimerManager().SetTimer(CharacterRotationTimerHandle, TimerDelegate, GetWorld()->DeltaTimeSeconds,true);
}

void AMYPawn::RotateSquadInTime(const float Duration,const FRotator CurrentRot, const FRotator DesiredRot)
{
	if (CharacterRotLerpVal <= Duration)
	{
		CharacterRotLerpVal += GetWorld()->GetDeltaSeconds();

		const FRotator NewRot = UKismetMathLibrary::RLerp(CurrentRot,DesiredRot, CharacterRotLerpVal / Duration, true);		
		
		SquadParent->SetRelativeRotation(NewRot);		
	}
	else
	{
		SquadParent->SetRelativeRotation(DesiredRot);
		bCanRotateCharacters = true;
		GetWorldTimerManager().ClearTimer(CharacterRotationTimerHandle);

		ChangeLeader();	
	}
}

#pragma endregion 

#pragma region Grenade Functions

void AMYPawn::GrenadeThrowStart()
{
	if(CurrentGrenadeCount == 0 || AliveMembersCount == 0) return;
	
	if (!bCanThrowGrenade)
	{
		bCanThrowGrenade = true;
		
		if (const auto PlayerCont = UGameplayStatics::GetPlayerController(GetWorld(),0))
		{
			PlayerCont->GetMousePosition(CachedMousePos.X,CachedMousePos.Y);
			
			FVector2d ScreenPos;
			UGameplayStatics::ProjectWorldToScreen(PlayerCont,GetActorLocation(),ScreenPos);
			PlayerCont->SetMouseLocation(ScreenPos.X,ScreenPos.Y);	
		}

		
		StartSlowMoTimer();		
		ChangeTimeDilations(true);//
	}
	else
	{
		GrenadeThrowEnd();
	}
}

void AMYPawn::ChangeTimeDilations(bool bUseSlowMo)
{
	if (bUseSlowMo)
	{
		UGameplayStatics::SetGlobalTimeDilation(this,0.1f);
		CustomTimeDilation = 5.0f;	
	}
	else
	{
		UGameplayStatics::SetGlobalTimeDilation(this,1.0f);
		CustomTimeDilation = 1.0f;
	}
	
}

void AMYPawn::ThrowGrenade()
{
	if(!GrenadeToSpawn) return;

	CurrentGrenadeCount--;
	OnGrenadeChanged.Broadcast(CurrentGrenadeCount,MaxGrenadeCount);
	
	StopSlowMoTimer();
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = this;
	if (const auto Grenade = GetWorld()->SpawnActor<AGrenadeBase>(GrenadeToSpawn,GetActorLocation(),FRotator::ZeroRotator,SpawnParameters))
	{
		FVector EndPos = SpringArm->GetComponentLocation();
		EndPos.Z = 0.0f;
		const TArray GrenadePathPositions{CalculateGrenadePath(GetActorLocation(),EndPos)};		
		
		Grenade->MoveToDestination(GrenadePathPositions);
	}

	GrenadeThrowEnd();
			
}

void AMYPawn::GrenadeThrowEnd()
{
	bCanThrowGrenade = false;

	if (const auto PlayerCont = UGameplayStatics::GetPlayerController(GetWorld(),0))
	{		
		PlayerCont->SetMouseLocation(CachedMousePos.X,CachedMousePos.Y);	
	}				

	SpringArm->SetRelativeLocation(FVector::Zero());

	ChangeTimeDilations(false);
}

void AMYPawn::VisualizeGrenadeTrajectory(const FVector StartPos, const FVector EndPos) const
{
	TArray<FVector> Positions = CalculateGrenadePath(StartPos,EndPos);

	for (int i = 1; i < Positions.Num(); ++i)
	{
		FVector Pos1 = Positions[i - 1];
		FVector Pos2 = Positions[i];		
	
		DrawDebugSphere(GetWorld(),Pos2,10,8,FColor::Green);
		DrawDebugLine(GetWorld(),Pos1,Pos2,FColor::Red);
	}
	
	DrawDebugCircle(GetWorld(),EndPos,GrenadeEffectRadius,32,FColor::Red,false,-1,0,2,FVector::ForwardVector,FVector::RightVector);
}

TArray<FVector> AMYPawn::CalculateGrenadePath(const FVector StartPos, const FVector EndPos) const
{
	FVector MidPos =  (EndPos + StartPos) * 0.5f;
	MidPos.Z += GrenadeThrowHeight;
	DrawDebugSphere(GetWorld(),MidPos,15,8,FColor::Black);

	TArray<FVector> Positions;
	for (float t = 0.0f; t <= 1.0f; t += 0.05f)
	{
		FVector P1 = FMath::Lerp(StartPos,MidPos, t);
		FVector P2 = FMath::Lerp(P1,EndPos, t);
		Positions.Add(P2); 	
	}

	Positions.Add(EndPos);

	return Positions;
}

#pragma endregion 

void AMYPawn::StartFire()
{
	if (bCanThrowGrenade)
	{
		ThrowGrenade();
		return;
	}
	
	bCanFire = true;
}

void AMYPawn::EndFire()
{
	bCanFire = false;
}

void AMYPawn::MemberDeath(bool bIsLeaderDead)
{
	AliveMembersCount--;

	// If the current leader is dead then change the leader
	if(bIsLeaderDead)
	{
		RotateSquadMembers();
	}

	if (AliveMembersCount == 0)
	{
		PawnMovementComp->StopActiveMovement();

		UGameplayStatics::SetGlobalTimeDilation(this,0.0f);

		OnAllMembersDead.Broadcast();

		PrimaryActorTick.bCanEverTick = false;

		// Notify GM that all members are dead
		if(const auto GM = Cast<AMYShootEmUpGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->GameOver();
		}
	}
}

void AMYPawn::GrenadePickedUp()
{
	CurrentGrenadeCount = FMath::Min(CurrentGrenadeCount + 1, MaxGrenadeCount);
	OnGrenadeChanged.Broadcast(CurrentGrenadeCount, MaxGrenadeCount);
}

void AMYPawn::ChangeLeader()
{
	if(AliveMembersCount == 0) return;
	
	CurrentLeaderIndex++;

	if(CurrentLeaderIndex == SquadMembers.Num()) CurrentLeaderIndex = 0;

	int PreviousLeaderIndex = CurrentLeaderIndex - 1;
	if (CurrentLeaderIndex == 0)
	{
		PreviousLeaderIndex = SquadMembers.Num() - 1;
	}


	if (SquadMembers[PreviousLeaderIndex] != nullptr)
	{
		if (USceneComponent* PrevLeaderRoot = SquadMembers[PreviousLeaderIndex]->GetRootComponent())
		{
			PrevLeaderRoot->ComponentTags.Empty();			
		}
	}
	if (SquadMembers[CurrentLeaderIndex] != nullptr)
	{
		if (USceneComponent* CurrLeaderRoot = SquadMembers[CurrentLeaderIndex]->GetRootComponent())
		{
			CurrLeaderRoot->ComponentTags.Add(FName("Leader"));
			OnLeaderChanged.Broadcast();
		}		
	}	
	else
	{
		ChangeLeader();
	}	
}

void AMYPawn::ChangeFormation()
{
	if(const APlayerController* PC = UGameplayStatics::GetPlayerController(this,0))
	{
		if(PC->IsInputKeyDown(EKeys::One))
		{
			CurrentFormationType = ESquadFormationType::ESF_BackToBack;
		}
		else if (PC->IsInputKeyDown(EKeys::Two))
		{
			CurrentFormationType = ESquadFormationType::ESF_LineH;
		}
		else if (PC->IsInputKeyDown(EKeys::Three))
		{
			CurrentFormationType = ESquadFormationType::ESF_LineV;
		}

		TArray<FVector> FormationPositions;
		TArray<FRotator> FormationRotations;

		for (const auto FormationData : SquadFormationData)
		{
			if (FormationData->FormationType == CurrentFormationType)
			{
				FormationPositions = FormationData->FormationPositions;
				FormationRotations = FormationData->FormationRotations;
			}
		}

		for (int i = 0; i < FormationPositions.Num() - 1; ++i)
		{
			// SquadMemberPositions[i]->SetRelativeLocation(FormationPositions[i]);
			// SquadMemberPositions[i]->SetRelativeRotation(FormationRotations[i]);
			if(!SquadMembers[i]) continue;

			SquadMembers[i]->SetActorRelativeLocation(FormationPositions[i]);
			SquadMembers[i]->SetActorRelativeRotation(FormationRotations[i]);
			
		}
	}
}

void AMYPawn::InitializeSquad()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FAttachmentTransformRules AttachmentTransformRules{EAttachmentRule::KeepRelative,EAttachmentRule::KeepRelative,EAttachmentRule::KeepWorld, true};

	const int ChildPosCount = SquadParent->GetNumChildrenComponents();

	SquadParent->GetChildrenComponents(false,SquadMemberPositions);
	
	for (int i = 0; i < ChildPosCount; ++i)
	{
		FVector Pos = SquadFormationData[0]->FormationPositions[i];
		const FRotator Rot = SquadFormationData[0]->FormationRotations[i];

		FTransform Transform{Rot,Pos,FVector::OneVector * 0.1f};
		
		SquadParent->GetChildComponent(i)->SetRelativeTransform(Transform);
		
		if(i <= 3) // Spawn squad members
		{
			if (AMYCharacterBase* Character = GetWorld()->SpawnActor<AMYCharacterBase>(
				SquadMembersData->SquadMembers[i], SpawnParameters))
			{
				Character->AttachToComponent(SquadParent, FAttachmentTransformRules::KeepRelativeTransform);

				//Pos.Z += Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

				Character->SetActorRelativeLocation(Pos);
				Character->SetActorRotation(Rot);

				SquadMembers.Add(Character);
			}
		}
		else // Spawn Drone
		{
			if (AActor* Drone = GetWorld()->SpawnActor<AActor>(SquadMembersData->Drone, SpawnParameters))
			{
				Drone->AttachToComponent(SquadParent, FAttachmentTransformRules::KeepRelativeTransform);

				Drone->SetActorRelativeLocation(Pos);
				Drone->SetActorRotation(Rot);
			}
		}
		//DrawDebugCapsule(GetWorld(),SquadParent->GetChildComponent(i)->GetComponentLocation(),96,30,FQuat{FVector::UpVector,90}, FColor::Cyan, true, 10); 
	}	
}


