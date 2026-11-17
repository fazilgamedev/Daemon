// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/MannequinAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "Actors/Items/Item.h"
#include "Camera/CameraComponent.h"
#include "Characters/BasePlayer.h"

void UMannequinAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	PlayerRef = Cast<ABasePlayer>(TryGetPawnOwner());
	if (!PlayerRef) return;
	PlayerRef->OnCurrentItemChangedSignature.AddDynamic(this, &UMannequinAnimInstance::OnCurrentItemChanged);
	
	
	CurrentItem = PlayerRef->CurrentItem;
	if (!CurrentItem) return;
	
}

void UMannequinAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!PlayerRef) return;
	// {
	// 	PlayerRef = Cast<ABasePlayer>(TryGetPawnOwner());
	// 	PlayerRef->OnCurrentItemChangedSignature.AddDynamic(this, &UMannequinAnimInstance::OnCurrentItemChanged);
	// 	return;
	// }
	Velocity = PlayerRef->GetVelocity();
	Speed = Velocity.Size2D();
	Direction = CalculateDirection(Velocity, PlayerRef->GetActorRotation());
	float p = PlayerRef->GetBaseAimRotation().Pitch;
	float t = (p > 180.f ? 360 - p : -p) / 3;
	Pitch = FMath::FInterpConstantTo(Pitch, t, DeltaSeconds, 200.f);
	
	if (!(CurrentItem && CurrentItem->ItemType == EItemType::Weapon))
	{
		SightTransform = RelativeHandTransform = FTransform::Identity;
		return;
	}
	SetSightTramsform();
	SetRelativeSightTransform();
}

void UMannequinAnimInstance::SetSightTramsform()
{
	if (!(PlayerRef && PlayerRef->CurrentItem)) return;
	FTransform tmp = PlayerRef->Camera->GetComponentTransform().GetRelativeTransform(PlayerRef->GetMesh()->GetSocketTransform(FName("head")));
	SightTransform = FTransform(tmp.GetRotation().Rotator(),  tmp.GetRotation().GetForwardVector() * PlayerRef->CurrentItem->SightOffset + tmp.GetLocation());
}

void UMannequinAnimInstance::SetRelativeSightTransform()
{
	if (!(PlayerRef && PlayerRef->CurrentItem)) return;
	RelativeHandTransform = PlayerRef->CurrentItem->ItemModel->GetSocketTransform(FName("S_Sight")).GetRelativeTransform(PlayerRef->GetMesh()->GetSocketTransform(FName("hand_r")));
}

void UMannequinAnimInstance::OnCurrentItemChanged()
{
	if (!(PlayerRef && PlayerRef->CurrentItem))
	{
		CurrentItem = nullptr;
		return;
	}
	CurrentItem = PlayerRef->CurrentItem;
}
