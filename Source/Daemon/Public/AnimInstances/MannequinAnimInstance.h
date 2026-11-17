// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MannequinAnimInstance.generated.h"

class AItem;
class ABasePlayer;
/**
 * 
 */
UCLASS()
class DAEMON_API UMannequinAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ABasePlayer* PlayerRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AItem* CurrentItem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Direction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Pitch;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform SightTransform;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform RelativeHandTransform;
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION()
	void SetSightTramsform();
	
	UFUNCTION()
	void SetRelativeSightTransform();
	
	UFUNCTION()
	void OnCurrentItemChanged();
};
