// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AirCraft.generated.h"

class USkeletalMeshComponent;

UCLASS()
class DAEMON_API AAirCraft : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAirCraft();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* AirCraftBody;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
