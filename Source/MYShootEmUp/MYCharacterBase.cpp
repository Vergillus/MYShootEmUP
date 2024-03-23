// Fill out your copyright notice in the Description page of Project Settings.


#include "MYCharacterBase.h"

#include "HealthComponent.h"
#include "MYPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/WeaponBase.h"

// Sets default values
AMYCharacterBase::AMYCharacterBase() :
	WeaponSocketName("hand_r_WeaponSocket"),
	bIsLeader(false),
	bIsSoloMode(false)

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Comp"));
	HealthComponent->OnDeath.AddDynamic(this,&AMYCharacterBase::OnDeathHandler);

	LineOfSightParent = CreateDefaultSubobject<USceneComponent>(TEXT("Line of Sight Parent"));
	LineOfSightParent->SetupAttachment(RootComponent);

	LineOfSightDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Line of Sight"));
	LineOfSightDecal->SetupAttachment(LineOfSightParent);
	LineOfSightDecal->DecalSize = FVector(128,100,10);
}

void AMYCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Spawn and equip default weapon	
	EquipWeapon(SpawnDefaultWeapon());	
}

// Called when the game starts or when spawned
void AMYCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	const FVector NewScale{CurrentWeapon->GetFireRange() / LineOfSightDecal->DecalSize.X,
						   (CurrentWeapon->GetSweepRadius() / LineOfSightDecal->DecalSize.Y) * 2.0f,1};
	LineOfSightParent->SetWorldScale3D(NewScale); 
}

// Called every frame
void AMYCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMYCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMYCharacterBase::Fire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->FireWeapon();
	}
}

void AMYCharacterBase::OnDeathHandler()
{
	// Clear parent
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,false));
	
	// Ragdoll
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	GetMesh()->SetSimulatePhysics(true);	
			
	// set Lifespan
	SetLifeSpan(2.0f);	

	// Notify squad
	if(AMYPawn* Pawn = Cast<AMYPawn>(UGameplayStatics::GetPlayerPawn(this,0)))
	{
		Pawn->MemberDeath(bIsLeader);
		Pawn->OnLeaderChanged.Remove(this,FName("OnLeaderChangedHandler"));
		DiscardWeapon();
	}
}

void AMYCharacterBase::EquipWeapon(const TSubclassOf<AWeaponBase> Weapon)
{
	//UE_LOG(LogTemp,Warning,TEXT("EQUIP WEAPON"));
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if(DefaultWeapon && DefaultWeapon != CurrentWeapon)
	{
		DiscardWeapon();
	}
	
	if (AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(Weapon,SpawnParameters))
	{	
		if (const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(WeaponSocketName))
		{
			HandSocket->AttachActor(NewWeapon, GetMesh());

			const FVector SocketLoc = NewWeapon->GetWeaponMesh()->GetSocketLocation(FName("GripPoint"));			
			NewWeapon->SetActorRelativeLocation(NewWeapon->GetActorLocation() - SocketLoc);
		}

		//DefaultWeapon->SetHidden(true);
		DefaultWeapon->GetWeaponMesh()->SetVisibility(false, true);
		CurrentWeapon = NewWeapon;	
		
		CurrentWeapon->OnMagazineEmpty.AddDynamic(this,&AMYCharacterBase::DiscardWeapon);

		const FVector NewScale{CurrentWeapon->GetFireRange() / LineOfSightDecal->DecalSize.X,
						   (CurrentWeapon->GetSweepRadius() / LineOfSightDecal->DecalSize.Y) * 2.0f,1};
		LineOfSightParent->SetWorldScale3D(NewScale); 
	}
}

void AMYCharacterBase::EquipWeapon(AWeaponBase* WeaponToEquip)
{
	if (WeaponToEquip)
	{
		// Get Hand Socket
		if (const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(WeaponSocketName))
		{
			HandSocket->AttachActor(WeaponToEquip,GetMesh());

			const FVector SocketLoc = WeaponToEquip->GetWeaponMesh()->GetSocketLocation(FName("GripPoint"));			
			WeaponToEquip->SetActorRelativeLocation(WeaponToEquip->GetActorLocation() - SocketLoc);
		}

		CurrentWeapon = WeaponToEquip;
		DefaultWeapon = CurrentWeapon;		
	}
}

void AMYCharacterBase::DiscardWeapon()
{
	CurrentWeapon->OnMagazineEmpty.RemoveDynamic(this,&AMYCharacterBase::DiscardWeapon);
	
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules{EDetachmentRule::KeepWorld,true});

	CurrentWeapon->DiscardWeapon();	
	
	if(DefaultWeapon)
	{
		CurrentWeapon = DefaultWeapon;
		//DefaultWeapon->SetHidden(false);
		DefaultWeapon->GetWeaponMesh()->SetVisibility(true, true);		
	}	
}

void AMYCharacterBase::OnLeaderChangedHandler()
{
	bIsLeader = RootComponent->ComponentHasTag(FName("Leader"));
}

AWeaponBase* AMYCharacterBase::SpawnDefaultWeapon() 
{
	if (DefaultWeaponToSpawn)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		return GetWorld()->SpawnActor<AWeaponBase>(DefaultWeaponToSpawn,SpawnParameters);		
	}
	return nullptr;
}

void AMYCharacterBase::SetVelocity_Implementation(const FVector NewVelocity)
{
	GetMovementComponent()->Velocity = NewVelocity;
}

void AMYCharacterBase::SubscribeToLeaderChange(FOnLeaderChanged& LeaderChanged)
{
	LeaderChanged.AddDynamic(this, &AMYCharacterBase::OnLeaderChangedHandler);	
}
