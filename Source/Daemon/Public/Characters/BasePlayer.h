// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BasePlayer.generated.h"

class UHealthComponent;
class UMannequinAnimInstance;
class UPostProcessComponent;
class AItem;
class AInteractable;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class ABasePlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCurrentItemChangedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAimStateChangedSignature, bool, bADS);

UCLASS()
class DAEMON_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasePlayer();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* IMC_Default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_Move;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_Look;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_Jump;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_Interact;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_DropCurrent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_UseItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_Aim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_Sprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPostProcessComponent* PPComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABasePlayerController* PC_Ref;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VSen;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HSen;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentItem)
	AItem* CurrentItem;
	
	UFUNCTION()
	void OnRep_CurrentItem();
	
	UPROPERTY()
	FOnCurrentItemChangedSignature OnCurrentItemChangedSignature;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMannequinAnimInstance* MannyAnimRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_bIsAiming)
	bool bIsAiming;

	UFUNCTION()
	void OnRep_bIsAiming();
	
	UPROPERTY()
	FOnAimStateChangedSignature OnAimStateChanged;
	
	UPROPERTY()
	float FrontMove;

	UPROPERTY()
	float SideMove;
	
	UPROPERTY()
	FVector WalkVector;
	
	UPROPERTY()
	float CurrentPhase;

	UPROPERTY()
	FVector CurrentAmp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimSequence*> DeathAnims;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AfterDeathDestroyDelay;
	
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& Value);
	
	
protected:
		
	UFUNCTION(Server, Reliable)
	void SR_Interact();
	
	UFUNCTION(Server, Reliable)
	void SR_DropCurrent();
	
	UFUNCTION(Server, Reliable)
	void SR_OnDeath();
	
	UFUNCTION(NetMulticast, Reliable)
	void MC_OnDeath();
	
	UFUNCTION(Server, Reliable)
	void SR_Use();
	
	UFUNCTION(Server, Reliable)
	void SR_StopUse();
	
	UFUNCTION(Server, Reliable)
	void SR_ADS();
	
	UFUNCTION(Server, Reliable)
	void SR_StopADS();
	
	UFUNCTION(Server, Reliable)
	void SR_Sprint();
	
	UFUNCTION(Server, Reliable)
	void SR_StopSprint();
	
	
	
};
