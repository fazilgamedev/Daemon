// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/MannequinAnimInstance.h"

#include "Characters/BasePlayer.h"

void UMannequinAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	PlayerRef = Cast<ABasePlayer>(TryGetPawnOwner());
	if (!PlayerRef) return;
	
}

void UMannequinAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!PlayerRef) { PlayerRef = Cast<ABasePlayer>(TryGetPawnOwner()); return; }
	Velocity = PlayerRef->GetVelocity();
	Speed = Velocity.Size2D();
	Direction = CalculateDirection(Velocity, PlayerRef->GetActorRotation());
	float p = PlayerRef->GetBaseAimRotation().Pitch;
	Pitch = -(p > 180.f ? p - 360.f : p) / 5;
}
