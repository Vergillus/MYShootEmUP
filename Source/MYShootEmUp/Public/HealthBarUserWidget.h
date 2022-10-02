// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBarUserWidget.generated.h"

class UHealthComponent;

/**
 * 
 */
UCLASS()
class MYSHOOTEMUP_API UHealthBarUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

public:

	UFUNCTION(BlueprintCallable)
	void SubscribeOnHealthChange(UHealthComponent* HealthComp);

	UFUNCTION(BlueprintCallable)
	void OnHealthChangedHandler(AActor* Instigator, float MaxHealth, float CurrentHealth);
	
};
