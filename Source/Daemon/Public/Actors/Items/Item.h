// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interactables/Interactable.h"
#include "Item.generated.h"

class USkeletalMeshComponent;

UCLASS()
class DAEMON_API AItem : public AInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* ItemModel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bIsHeld;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Socket;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void OnInteract_Implementation(ABasePlayer* Interactor) override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDetach(ABasePlayer* Interactor, float VelocityToDetachAt);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Use(ABasePlayer* User);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopUse();
};
