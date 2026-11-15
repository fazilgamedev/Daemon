// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Door.generated.h"

class UStaticMeshComponent;

UCLASS()
class DAEMON_API ADoor : public AInteractable
{
	GENERATED_BODY()

public:		
	// Sets default values for this actor's properties
	ADoor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DoorMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOpen;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	FTimerHandle DoorTimer;
	
	UFUNCTION(BlueprintCallable)
	void RotateDoor();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnInteract_Implementation(ABasePlayer* Interactor) override;
};
