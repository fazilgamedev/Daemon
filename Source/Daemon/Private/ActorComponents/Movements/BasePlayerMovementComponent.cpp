// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/Movements/BasePlayerMovementComponent.h"

#include "GameFramework/Character.h"

bool UBasePlayerMovementComponent::FSavedMove::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter,
                                                              float MaxDelta) const
{
	const FSavedMove* NextMove = static_cast<const FSavedMove*>(NewMove.Get());
	if (bSavedSprint != NextMove->bSavedSprint) return false;
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UBasePlayerMovementComponent::FSavedMove::Clear()
{
	Super::Clear();
	bSavedSprint = false;
}

uint8 UBasePlayerMovementComponent::FSavedMove::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();
	if (bSavedSprint) Result |= FLAG_Custom_0;
	return Result;
}

void UBasePlayerMovementComponent::FSavedMove::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel,
	class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
	if (const UBasePlayerMovementComponent* MovComp = Cast<UBasePlayerMovementComponent>(C->GetCharacterMovement()))
	{
		bSavedSprint = MovComp->bWantsSprint;
	}
}

void UBasePlayerMovementComponent::FSavedMove::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);
	if (UBasePlayerMovementComponent* MovComp = Cast<UBasePlayerMovementComponent>(C->GetCharacterMovement()))
	{
		MovComp->bWantsSprint = bSavedSprint;
	}
}

UBasePlayerMovementComponent::FClientPredictionData::FClientPredictionData(
	const UCharacterMovementComponent& ClientMovementComponent) : Super(ClientMovementComponent)
{
	
}

FSavedMovePtr UBasePlayerMovementComponent::FClientPredictionData::AllocateNewMove()
{
	return MakeShared<FSavedMove>();
}

UBasePlayerMovementComponent::UBasePlayerMovementComponent()
{
	MaxWalkSpeed = WalkSpeed;
	bWantsSprint = false;
}

float UBasePlayerMovementComponent::GetMaxSpeed() const
{
	if (bWantsSprint && MovementMode == EMovementMode::MOVE_Walking && !IsCrouching()) return SprintSpeed;
	return Super::GetMaxSpeed();
}

void UBasePlayerMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	
	bWantsSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

FNetworkPredictionData_Client* UBasePlayerMovementComponent::GetPredictionData_Client() const
{
	if (!ClientPredictionData)
	{
		UBasePlayerMovementComponent* MutableThis = const_cast<UBasePlayerMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FClientPredictionData(*this);
	}
	return ClientPredictionData;
}

void UBasePlayerMovementComponent::SetSprint(bool bEnable)
{
	bWantsSprint = bEnable;
}
