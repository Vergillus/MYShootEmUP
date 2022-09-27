// Fill out your copyright notice in the Description page of Project Settings.


#include "MYPawn.h"

#include "HealthComponent.h"
#include "MYCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMYPawn::AMYPawn() :
	CharacterRotationDuration(0.25f),
	bCanRotateCharacters(true),
	bCanThrowGrenade(false),
	bCanFire(false)
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

	RotParentByMouse = CreateDefaultSubobject<USceneComponent>(TEXT("Rotate Squad by Mouse"));
	RotParentByMouse->SetupAttachment(RootComponent);

	SquadParent = CreateDefaultSubobject<USceneComponent>(TEXT("Squad Parent"));
	SquadParent->SetupAttachment(RotParentByMouse);	

	constexpr int SquadMemberCnt = 4;
	
	FString SocketName{};
    for (int i = 0; i < SquadMemberCnt; ++i)
    {
    	SocketName = "Pos_" + FString::FromInt(i);
        if (UStaticMeshComponent* SceneComp = CreateDefaultSubobject<UStaticMeshComponent>(FName(*SocketName)))
        {
	        SceneComp->SetupAttachment(SquadParent);
        	SceneComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        	SquadMemberPositions.Add(SceneComp);
        }
    }	

    for (int i = 0; i < SquadMemberCnt; ++i)
    {
    	SocketName = "Character_" + FString::FromInt(i);
	    if (UChildActorComponent* ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(FName(*SocketName)))
	    {
	    	ChildActorComponent->SetupAttachment(SquadMemberPositions[i]);
	    	ChildActorComponent->SetUsingAbsoluteScale(true);	 
	    	ChildActorComponent->SetRelativeLocation(FVector::UpVector * 90);
	    	ChildActorComponent->CreateChildActor();

	    	SquadMembers.Add(ChildActorComponent);
	    }
    }

	PawnMovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Comp"));
}

// Called when the game starts or when spawned
void AMYPawn::BeginPlay()
{
	Super::BeginPlay();
	
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
		DrawDebugSphere(GetWorld(),LinePlaneIntersectionPoint, 25.0f, 6, FColor::Cyan);
		FVector RotateDirection = (LinePlaneIntersectionPoint - GetActorLocation()).GetSafeNormal();
		RotateDirection.Z = 0.0f;

		if(!bCanThrowGrenade)
			RotParentByMouse->SetRelativeRotation(UKismetMathLibrary::MakeRotFromX(RotateDirection)); 
	}

	if (bCanThrowGrenade)
	{
		DrawDebugCircle(GetWorld(),GetActorLocation(),1500,16,FColor::Blue,false,-1,0,2,FVector::ForwardVector,FVector::RightVector);
		DrawDebugSphere(GetWorld(),SpringArm->GetRelativeLocation(), 15.0f, 6, FColor::Magenta);

		FVector SpringArmNewPos = FMath::Lerp(SpringArm->GetRelativeLocation(), LinePlaneIntersectionPoint, DeltaTime * 0.5f);
		SpringArmNewPos = SpringArmNewPos.GetClampedToSize(0,1500);
		SpringArm->SetRelativeLocation(SpringArmNewPos);
	}
	
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

}

void AMYPawn::MoveForwardBack(float AxisVal)
{
	FVector CameraForward = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0)->GetActorForwardVector();
	CameraForward.Z = 0;
	PawnMovementComp->AddInputVector(CameraForward * AxisVal);
}

void AMYPawn::MoveLeftRight(float AxisVal)
{
	const FVector CameraRight = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0)->GetActorRightVector();	
	PawnMovementComp->AddInputVector(CameraRight * AxisVal);
}

void AMYPawn::RotateSquadMembers()
{
	if(!bCanRotateCharacters) return;

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
	}
}


void AMYPawn::GrenadeThrowStart()
{
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
	}
	else
	{
		GrenadeThrowEnd();
	}
}

void AMYPawn::ThrowGrenade()
{
		
}

void AMYPawn::GrenadeThrowEnd()
{
	bCanThrowGrenade = false;

	if (const auto PlayerCont = UGameplayStatics::GetPlayerController(GetWorld(),0))
	{		
		PlayerCont->SetMouseLocation(CachedMousePos.X,CachedMousePos.Y);	
	}				

	SpringArm->SetRelativeLocation(FVector::Zero());
}

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
