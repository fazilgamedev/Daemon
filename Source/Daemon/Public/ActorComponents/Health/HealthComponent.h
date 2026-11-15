// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class AActor;
class UDamageType;
class AController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedSignature, UHealthComponent*, HealthComponent, float, Health, float, HealthDelta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DAEMON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Health)
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	
	UPROPERTY()
	bool bIsDead;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChangedSignature;
	
	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeathSignature;
	
	UFUNCTION()
	void OnRep_Health(float OldHealth);
	
	UFUNCTION(NetMulticast, Reliable)
	void MC_OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION(Server, Reliable)
	void SR_OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(NetMulticast, Reliable)
	void MC_Heal(float HealAmount);
	
	UFUNCTION(Server, Reliable)
	void SR_Heal(float HealAmount);
	
	UFUNCTION(BlueprintCallable)
	void Heal(float HealAmount);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() const { return Health; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth() const { return MaxHealth; }
	
	UFUNCTION()
	void DefaultDeath();

};
