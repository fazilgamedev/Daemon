// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/AirCraftAnimInstance.h"

#include "Actors/AirCrafts/AirCraft.h"

void UAirCraftAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	AirCraftRef = Cast<AAirCraft>(TryGetPawnOwner());
	if (!AirCraftRef) return;
	
}

void UAirCraftAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!AirCraftRef) { AirCraftRef = Cast<AAirCraft>(TryGetPawnOwner()); return; }
	RotorRotation = FMath::Fmod((DeltaSeconds * Speed) + RotorRotation, 360.f);
}
