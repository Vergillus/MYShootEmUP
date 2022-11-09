// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MYCharacterBase.generated.h"

class FOnLeaderChanged;
class UHealthComponent;

UCLASS()
class MYSHOOTEMUP_API AMYCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMYCharacterBase();	

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Components", meta=(AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Components", meta=(AllowPrivateAccess = "true"))
	UChildActorComponent* DefaultWeapon;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadWrite)
	class AWeaponBase* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category= "Weapon")
	FName WeaponSocketName;

	UFUNCTION()
	void OnDeathHandler();	

	/* Event handler function. Called when CurrentWeapon has 0 magazine capacity */
	UFUNCTION()
	void DiscardWeapon();

	UFUNCTION(BlueprintCallable)
	void CalculateWeaponPosition();

	UPROPERTY(BlueprintReadOnly)
	bool bIsLeader;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsSoloMode;

	UFUNCTION()
	void OnLeaderChangedHandler();	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Fire();

	void EquipWeapon(const TSubclassOf<AWeaponBase> Weapon);

	FORCEINLINE AWeaponBase* GetCurrentWeapon() const {return CurrentWeapon;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsSoloMode() const
	{
		return bIsSoloMode;
	}

	UFUNCTION(BlueprintNativeEvent)
	void SetVelocity(const FVector NewVelocity);	

	void SubscribeToLeaderChange(FOnLeaderChanged& LeaderChanged);

};

