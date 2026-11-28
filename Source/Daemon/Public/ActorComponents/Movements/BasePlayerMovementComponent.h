// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BasePlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DAEMON_API UBasePlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	class FSavedMove : public FSavedMove_Character
	{
	public:
		typedef FSavedMove_Character Super;
		
		uint8 bSavedSprint : 1;
		
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};
	
	class FClientPredictionData : public FNetworkPredictionData_Client_Character
	{
	public:
		typedef FNetworkPredictionData_Client_Character Super;
		
		FClientPredictionData(const UCharacterMovementComponent& ClientMovementComponent);
		
		virtual FSavedMovePtr AllocateNewMove() override;
	};
	
public:
	
	UBasePlayerMovementComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float WalkSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeed = 1000.f;
	
private:
	 
	UPROPERTY()
	bool bWantsSprint;
	
public:
	
	virtual float GetMaxSpeed() const override;
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void SetSprint(bool bEnable);
	
	
	
	
};
