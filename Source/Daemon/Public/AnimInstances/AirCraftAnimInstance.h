// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AirCraftAnimInstance.generated.h"

class AAirCraft;
/**
 * 
 */
UCLASS()
class DAEMON_API UAirCraftAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AAirCraft* AirCraftRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RotorRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
