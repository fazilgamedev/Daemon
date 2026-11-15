// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interactables/Interactable.h"

#include "Characters/BasePlayer.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AInteractable::AInteractable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicateMovement(true);
	
	PickupModel = CreateDefaultSubobject<UStaticMeshComponent>(FName("PickupModel"));
	SetRootComponent(PickupModel);
	PickupModel->SetSimulatePhysics(true);
	// PickupModel->SetMassOverrideInKg(NAME_None, 100.f);
	PickupModel->SetAngularDamping(.45f);
	PickupModel->SetLinearDamping(.45f);
	PickupModel->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupModel->SetCollisionResponseToAllChannels(ECR_Block);
	PickupModel->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleCollision"));
	CapsuleCollision->SetupAttachment(RootComponent);
	CapsuleCollision->SetCapsuleHalfHeight(100.f);
	CapsuleCollision->SetCapsuleRadius(100.f);
	
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	//SetOwner(nullptr);

}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractable::OnInteract_Implementation(ABasePlayer* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("The Object being interacted on is %s"), *GetName());
}


