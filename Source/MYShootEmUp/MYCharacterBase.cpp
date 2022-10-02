// Fill out your copyright notice in the Description page of Project Settings.


#include "MYCharacterBase.h"

#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/WeaponBase.h"

// Sets default values
AMYCharacterBase::AMYCharacterBase() :
	WeaponSocketName("hand_r_WeaponSocket")

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Comp"));
	HealthComponent->OnDeath.AddDynamic(this,&AMYCharacterBase::OnDeathHandler);

	DefaultWeapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Default Weapon"));
	DefaultWeapon->SetupAttachment(GetMesh(),WeaponSocketName);
}

// Called when the game starts or when spawned
void AMYCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if(AWeaponBase* Weapon = Cast<AWeaponBase>(DefaultWeapon->GetChildActor()))
	{
		CurrentWeapon = Weapon;
		CurrentWeapon->SetIsDefaultWeapon(true);
		CurrentWeapon->SetOwner(this);
	}	
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
	
	// Play Death montage
	if (DeathMontage)
	{
		
	}

	// Notify squad
}

void AMYCharacterBase::EquipWeapon(const TSubclassOf<AWeaponBase> Weapon)
{
	UE_LOG(LogTemp,Warning,TEXT("EQUIP WEAPON"));
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const AWeaponBase* DefWeapon = Cast<AWeaponBase>(DefaultWeapon->GetChildActor());
	if(DefWeapon && DefWeapon != CurrentWeapon)
	{
		DiscardWeapon();
	}
	
	if (AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(Weapon,SpawnParameters))
	{
		const FAttachmentTransformRules AttachmentTransformRules{EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,true};
		NewWeapon->AttachToComponent(GetMesh(),AttachmentTransformRules,WeaponSocketName);
		
		DefaultWeapon->SetVisibility(false, true);
		CurrentWeapon = NewWeapon;
		
		CurrentWeapon->SetActorRelativeLocation(FVector::Zero());
		CurrentWeapon->OnMagazineEmpty.AddDynamic(this,&AMYCharacterBase::DiscardWeapon);
		
		CalculateWeaponPosition();
	}
}

void AMYCharacterBase::DiscardWeapon()
{
	CurrentWeapon->OnMagazineEmpty.RemoveDynamic(this,&AMYCharacterBase::DiscardWeapon);
	
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules{EDetachmentRule::KeepWorld,true});

	CurrentWeapon->DiscardWeapon();	
	
	if(AWeaponBase* Weapon = Cast<AWeaponBase>(DefaultWeapon->GetChildActor()))
	{
		CurrentWeapon = Weapon;
		DefaultWeapon->SetVisibility(true, true);
	}	
}

void AMYCharacterBase::CalculateWeaponPosition()
{
	if(!CurrentWeapon) return;	
	
	const FVector WeaponGripPos = CurrentWeapon->GetWeaponMesh()->GetSocketLocation(FName("GripPoint"));

	const FVector NewPos = CurrentWeapon->GetActorLocation() - WeaponGripPos;	

	CurrentWeapon->SetActorRelativeLocation(NewPos);	
}

