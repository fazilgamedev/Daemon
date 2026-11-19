// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/MannequinAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "Actors/Items/Item.h"
#include "Camera/CameraComponent.h"
#include "Characters/BasePlayer.h"
#include "Interfaces/WeaponInterface.h"

void UMannequinAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	PlayerRef = Cast<ABasePlayer>(TryGetPawnOwner());
	if (!PlayerRef) return;
	PlayerRef->OnCurrentItemChangedSignature.AddDynamic(this, &UMannequinAnimInstance::OnCurrentItemChanged);
	PlayerRef->OnAimStateChanged.AddDynamic(this, &UMannequinAnimInstance::OnADSChanged);
	OldRotation = PlayerRef->GetControlRotation();
	
	
	CurrentItem = PlayerRef->CurrentItem;
	if (!CurrentItem) return;
	
}

void UMannequinAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!PlayerRef) return;
	Velocity = PlayerRef->GetVelocity();
	Speed = Velocity.Size2D();
	Direction = CalculateDirection(Velocity, PlayerRef->GetActorRotation());
	
	// const FRotator TargetAimRotation = PlayerRef->IsLocallyControlled()
	// 	? PlayerRef->GetControlRotation()
	// 	: PlayerRef->GetBaseAimRotation();
	// AimRotation = PlayerRef->IsLocallyControlled()
	// 	? TargetAimRotation
	// 	: FMath::RInterpTo(AimRotation, TargetAimRotation, DeltaSeconds, 15.f);
	
	AimRotation = PlayerRef->IsLocallyControlled() ? PlayerRef->GetControlRotation() : PlayerRef->GetBaseAimRotation();
	float p = AimRotation.Pitch;
	Pitch = (p > 180.f ? 360 - p : -p) / 3;
	
	DirectionalRotation(SideMove, PlayerRef->SideMove, DeltaSeconds, 1.f, 6.f, 11.f);
	DirectionalRotation(FrontMove, PlayerRef->FrontMove, DeltaSeconds, 1.f, 4.f, 10.f);

	WalkAnim(DeltaSeconds);

	HandSway(DeltaSeconds);
	
	if (!(CurrentItem && CurrentItem->ItemType == EItemType::Weapon))
	{
		SightTransform = RelativeHandTransform = LHIKTransform = FTransform::Identity;
		LHIKAlpha = 0.f;
		return;
	}
	
	LHIKAlpha = 1.f;
	SetSightTransform();
	SetRelativeHandTransform();
	SetLHIKTransform();
	if (bInterpAiming) InterpAiming(DeltaSeconds);
	if (!RecoilTransform.Equals(FTransform::Identity) || !RecoilResetTransform.Equals(FTransform::Identity)) {
		InterpResetRecoil(DeltaSeconds);
		InterpRecoil(DeltaSeconds);
	}
}

void UMannequinAnimInstance::SetSightTransform()
{
	if (!(PlayerRef && CurrentItem)) return;
	const FTransform Tmp = PlayerRef->Camera->GetComponentTransform().GetRelativeTransform(PlayerRef->GetMesh()->GetSocketTransform(FName("head")));
	// const FTransform Tmp = FTransform(AimRotation, PlayerRef->Camera->GetComponentTransform().GetLocation()).GetRelativeTransform(PlayerRef->GetMesh()->GetSocketTransform(FName("head")));
	SightTransform = FTransform(Tmp.GetRotation().Rotator(),  Tmp.GetRotation().GetForwardVector() * CurrentItem->SightOffset + Tmp.GetLocation());
}

void UMannequinAnimInstance::SetRelativeHandTransform()
{
	if (!(PlayerRef && CurrentItem)) return;
	RelativeHandTransform = PlayerRef->CurrentItem->ItemModel->GetSocketTransform(FName("S_Sight")).GetRelativeTransform(PlayerRef->GetMesh()->GetSocketTransform(FName("hand_r")));
}

void UMannequinAnimInstance::SetLHIKTransform()
{
	if (!(PlayerRef && CurrentItem)) return;
	LHIKTransform = CurrentItem->ItemModel->GetSocketTransform(FName("S_LHIK")).GetRelativeTransform(PlayerRef->GetMesh()->GetSocketTransform(FName("hand_r")));
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

void UMannequinAnimInstance::InterpAiming(float DeltaTime)
{
	AimAlpha = FMath::FInterpTo(AimAlpha, bIsAiming ? 1.f : .95f /*0.f*/, DeltaTime, 10.f);
	if (AimAlpha <= .95f /*0.f*/ || AimAlpha >= 1.f) bInterpAiming = false;
}

void UMannequinAnimInstance::OnADSChanged(bool bADS)
{
	if (bIsAiming == bADS) return;
	bIsAiming = bADS;
	bInterpAiming = true;
}

void UMannequinAnimInstance::WalkAnim(float DeltaTime)
{
	SwayVector = FMath::VInterpTo(SwayVector, PlayerRef->WalkVector, DeltaTime, 10.f);
}

void UMannequinAnimInstance::HandSway(float DeltaTime)
{
	FRotator CurrentRotation = PlayerRef->GetControlRotation();
	// Clamp if needed
	TurnRotation = FMath::RInterpTo(TurnRotation, CurrentRotation - OldRotation, DeltaTime, 10.f);
	TurnRotation.Roll = TurnRotation.Pitch * -1.f;
	TurnRotation.Yaw = FMath::Clamp(TurnRotation.Yaw, -9.f, 9.f);
	TurnRotation.Roll = FMath::Clamp(TurnRotation.Roll, -9.f, 9.f);
	TurnLocation.X = TurnRotation.Yaw / 18.f;
	TurnLocation.Z = TurnRotation.Roll / 18.f;
	OldRotation = CurrentRotation;
}

void UMannequinAnimInstance::DirectionalRotation(float & DirVar, float MoveVar, float DeltaTime, float U, float V, float W)
{
	DirVar = FMath::FInterpTo(DirVar, UKismetMathLibrary::MapRangeClamped(MoveVar, -U, U, -V, V), DeltaTime, W);
}

void UMannequinAnimInstance::InterpRecoil(float DeltaTime)
{
	RecoilResetTransform = UKismetMathLibrary::TInterpTo(RecoilResetTransform, FTransform(), DeltaTime, 10.f);
}

void UMannequinAnimInstance::InterpResetRecoil(float DeltaTime)
{
	RecoilTransform = UKismetMathLibrary::TInterpTo(RecoilTransform, RecoilResetTransform, DeltaTime, 10.f);
}

void UMannequinAnimInstance::Firing()
{
	// if (CurrentItem && CurrentItem->StaticClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Firing"));
		TArray<float> RecoilVals = IWeaponInterface::Execute_GetRecoilVals(CurrentItem);
		FVector RecoilLocation = RecoilResetTransform.GetLocation();
		RecoilLocation += FVector(FMath::RandRange(-RecoilVals[4], RecoilVals[4]), FMath::RandRange(RecoilVals[3], -RecoilVals[3]), FMath::RandRange(-RecoilVals[5], RecoilVals[5]));
		FRotator RocoilRotation = RecoilResetTransform.GetRotation().Rotator();
		RocoilRotation += FRotator(FMath::RandRange(-RecoilVals[1], RecoilVals[1]), FMath::RandRange(-RecoilVals[2], RecoilVals[2]), FMath::RandRange(RecoilVals[0], -RecoilVals[0]));
		RecoilResetTransform.SetRotation(RocoilRotation.Quaternion());
		RecoilResetTransform.SetLocation(RecoilLocation);
	}
}

