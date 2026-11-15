// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MannequinAnimInstance.generated.h"

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
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Direction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Pitch;
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
