// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Interfaces/WeaponInterface.h"
#include "Weapon.generated.h"

UENUM()
enum class EFireSelect : uint8 {
	None,
	SemiAuto,
	FullAuto
};

UENUM()
enum class EWeaponType : uint8 {
	None,
	Melee,
	Projectile
};

class UParticleSystem;
class ABasePlayer;

UCLASS()
class DAEMON_API AWeapon : public AItem, public IWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFireSelect FireSelect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* HitEfx;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VerticalRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HorizontalRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SideRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FrontRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RightRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UpRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recoil_Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recoil_Horizontal_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recoil_Horizontal_Right;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> RecoilVals;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bTraceDebug = false;
	
	UPROPERTY()
	FTimerHandle UseHandle;
	
	// UPROPERTY()
	FTimerDelegate UseDelegate;
	
	UPROPERTY()
	FHitResult OutHit;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Use_Implementation(ABasePlayer* User) override;
	
	virtual void StopUse_Implementation() override;
	
	UFUNCTION()
	void LineTrace(ABasePlayer* User);
	
	UFUNCTION(NetMulticast, Reliable)
	void MC_Fire(ABasePlayer* User, bool bHit, FVector SpawnLocation, FRotator SpawnRotation);
	
	UFUNCTION()
	void AddRecoil(ABasePlayer* User);
	
	virtual TArray<float> GetRecoilVals_Implementation() override;
	
	virtual float GetSightOffset_Implementation() override;
	
	
};
