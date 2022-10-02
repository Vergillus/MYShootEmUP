// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarUserWidget.h"

#include "MYShootEmUp/HealthComponent.h"

void UHealthBarUserWidget::SubscribeOnHealthChange(UHealthComponent* HealthComp)
{
	HealthComp->OnHealthChanged.AddDynamic(this,&UHealthBarUserWidget::OnHealthChangedHandler);
	OnHealthChangedHandler(nullptr, 1,1);
}

void UHealthBarUserWidget::OnHealthChangedHandler(AActor* Instigator, float MaxHealth, float CurrentHealth)
{
	HealthBar->SetPercent(CurrentHealth / MaxHealth);
}
