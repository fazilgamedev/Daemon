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
	FRotator AimRotation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Pitch;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform SightTransform;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform RelativeHandTransform;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform LHIKTransform;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float LHIKAlpha;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimAlpha;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinAimAlpha;
	
	UPROPERTY()
	bool bInterpAiming;
	
	UPROPERTY()
	bool bIsAiming;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector SwayVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRotator TurnRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector TurnLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SideMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FrontMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTransform RecoilTransform;
	
	UPROPERTY()
	FRotator OldRotation;

	UPROPERTY()
	FTransform RecoilResetTransform;
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUninitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION()
	void SetSightTransform();
	
	UFUNCTION()
	void SetRelativeHandTransform();
	
	UFUNCTION()
	void SetLHIKTransform();
	
	UFUNCTION()
	void OnCurrentItemChanged();
	
	UFUNCTION()
	void InterpAiming(float DeltaTime);
	
	UFUNCTION()
	void OnADSChanged(bool bADS);
	
	
	UFUNCTION()
	void WalkAnim(float DeltaTime);

	UFUNCTION()
	void HandSway(float DeltaTime);

	UFUNCTION()
	void DirectionalRotation(float &DirVar, float MoveVar, float DeltaTime, float U, float V, float W);

	UFUNCTION()
	void InterpRecoil(float DeltaTime);

	UFUNCTION()
	void InterpResetRecoil(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void Firing();

	
};