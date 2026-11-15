// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AirCrafts/AirCraft.h"


// Sets default values
AAirCraft::AAirCraft()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	SetReplicateMovement(true);
	
	AirCraftBody = CreateDefaultSubobject<USkeletalMeshComponent>(FName("AirCraftBody"));
	SetRootComponent(AirCraftBody);
	
}

// Called when the game starts or when spawned
void AAirCraft::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAirCraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorWorldOffset(DeltaTime * Speed * GetActorForwardVector());
	
}

