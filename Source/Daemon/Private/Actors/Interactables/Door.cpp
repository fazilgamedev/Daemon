// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interactables/Door.h"

#include "Actors/Items/Item.h"
#include "Characters/BasePlayer.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);
	
	TargetYaw = 0.f;
}

void ADoor::OnRep_TargetYaw()
{
	GetWorldTimerManager().SetTimer(DoorTimer, this, &ADoor::RotateDoor, .01f, true);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::RotateDoor()
{
	float DoorYaw= DoorMesh->GetRelativeRotation().Yaw;
	if (!FMath::IsNearlyEqual(DoorYaw, TargetYaw, 1.f))
	{
		float NewYaw = FMath::FInterpTo(DoorYaw, TargetYaw, .01f, 5.f);
		DoorMesh->SetRelativeRotation(FRotator(0.f, NewYaw, 0.f));
	}else
	{
		DoorMesh->SetRelativeRotation(FRotator(0.f, TargetYaw, 0.f));
		GetWorldTimerManager().ClearTimer(DoorTimer);
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ADoor, TargetYaw);
}

void ADoor::OnInteract_Implementation(ABasePlayer* Interactor)
{
	Super::OnInteract_Implementation(Interactor);
	
	if (!(HasAuthority() && Interactor)) return;
	if (FMath::IsNearlyZero(TargetYaw, 1.f))
	{
		TargetYaw = (FVector::DotProduct(GetActorForwardVector(), (Interactor->GetActorLocation() - GetActorLocation()).GetSafeNormal())) >= 0.f ? -90 : 90.f;
	}else
	{
		TargetYaw = 0.f;
	}
	OnRep_TargetYaw();
}

