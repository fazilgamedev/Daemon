// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/UI/HealthBar.h"

#include "ActorComponents/Health/HealthComponent.h"
#include "Components/ProgressBar.h"

void UHealthBar::OnHealthChanged(UHealthComponent* HealthComponent, float Health, float HealthDelta)
{
	if (!(HealthBar && CachedHealthComp)) return;
	float Max = CachedHealthComp->GetMaxHealth();
	float Cur = CachedHealthComp->GetHealth();
	if (Max > 0.f) HealthBar->SetPercent(Cur / Max);	
}

void UHealthBar::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (!CachedHealthComp) return;
	CachedHealthComp->OnHealthChangedSignature.RemoveDynamic(this, &UHealthBar::OnHealthChanged);
}

void UHealthBar::InitHealthBar(UHealthComponent* HealthComponent)
{
	if (!HealthComponent) return;
	CachedHealthComp = HealthComponent;
	CachedHealthComp->OnHealthChangedSignature.AddDynamic(this, &UHealthBar::OnHealthChanged);
	float Max = CachedHealthComp->GetMaxHealth();
	float Cur = CachedHealthComp->GetHealth();
	if (HealthBar && Max > 0.f) HealthBar->SetPercent(Cur / Max);
}
