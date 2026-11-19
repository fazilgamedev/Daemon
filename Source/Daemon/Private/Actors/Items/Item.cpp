// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Item.h"

#include "Characters/BasePlayer.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ItemModel = CreateDefaultSubobject<USkeletalMeshComponent>(FName("ItemModel"));
	ItemModel->SetupAttachment(RootComponent);
	ItemModel->SetVisibility(false);
	ItemModel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AItem, bIsHeld);
}

void AItem::OnInteract_Implementation(ABasePlayer* Interactor)
{
	Super::OnInteract_Implementation(Interactor);
	if (GetOwner()) return;
	if (!Interactor) return;
	PickupModel->SetSimulatePhysics(false);
	PickupModel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(Interactor->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
	SetOwner(Interactor);
	PickupModel->SetVisibility(false);
	ItemModel->SetVisibility(true);
	if (Interactor->CurrentItem) Interactor->SR_DropCurrent_Implementation();
	Interactor->CurrentItem = this;
	if (HasAuthority()) Interactor->OnRep_CurrentItem();
	// AddTickPrerequisiteComponent(Interactor->GetMesh());
}

void AItem::StopUse_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Stopped Using"));
}

void AItem::Use_Implementation(ABasePlayer* User)
{
	if (!User || !User->IsValidLowLevel()) return;
	UE_LOG(LogTemp, Warning, TEXT("Using"));
}

void AItem::OnDetach_Implementation(ABasePlayer* Interactor, float VelocityToDetachAt)
{
	if (!GetOwner()) return;
	if (!Interactor) return;
	ItemModel->SetVisibility(false);
	PickupModel->SetVisibility(true);
	PickupModel->SetSimulatePhysics(true);
	PickupModel->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PickupModel->AddImpulse(Interactor->GetActorForwardVector() * VelocityToDetachAt, NAME_None, true);
	Interactor->CurrentItem = nullptr;
	if (HasAuthority()) Interactor->OnRep_CurrentItem();
	SetOwner(nullptr);
	// RemoveTickPrerequisiteComponent(Interactor->GetMesh());
}

