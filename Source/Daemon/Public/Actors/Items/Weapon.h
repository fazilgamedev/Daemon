// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
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

UCLASS()
class DAEMON_API AWeapon : public AItem
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
	
	UFUNCTION(Server, Reliable)
	void SR_Use(ABasePlayer* User);
	
	UFUNCTION(Server, Reliable)
	void SR_StopUse();
	
	UFUNCTION()
	void LineTrace(ABasePlayer* User);
};
