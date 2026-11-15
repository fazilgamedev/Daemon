// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interactables/Door.h"

#include "Actors/Items/Item.h"


// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::RotateDoor()
{
	float DoorYaw= DoorMesh->GetRelativeRotation().Yaw;
	float Target = bIsOpen ? 0.f : 90.f;
	if (!FMath::IsNearlyEqual(DoorYaw, Target, 1.f))
	{
		float NewYaw = FMath::FInterpTo(DoorYaw, Target, GetWorld()->GetDeltaSeconds(), 5.f);
		DoorMesh->SetRelativeRotation(FRotator(0.f, NewYaw, 0.f));
		GetWorldTimerManager().SetTimer(DoorTimer, this, &ADoor::RotateDoor, .01f, true);
	}else if (DoorYaw > 85.f)
	{
		bIsOpen = true;
		GetWorldTimerManager().ClearTimer(DoorTimer);
	}else if (DoorYaw < 5.f)
	{
		bIsOpen = false;
		GetWorldTimerManager().ClearTimer(DoorTimer);
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::OnInteract_Implementation(ABasePlayer* Interactor)
{
	// Super::OnInteract_Implementation(Interactor);
	RotateDoor();
}

