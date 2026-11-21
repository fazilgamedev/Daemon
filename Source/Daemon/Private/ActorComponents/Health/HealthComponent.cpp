// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/Health/HealthComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
	SetIsReplicatedByDefault(true);
	
	MaxHealth = 100.f;
	Health = MaxHealth;
	bIsDead = false;
	bAutoDestroyOnOwnerDeath = true;

}


void UHealthComponent::OnRep_Health(float OldHealth)
{
	float Delta = Health - OldHealth;
	OnHealthChangedSignature.Broadcast(this, Health, Delta);
	if (Health <= 0.f && !bIsDead)
	{
		bIsDead = true;
		OnDeathSignature.Broadcast();
	}
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	Health = MaxHealth;
	if (!GetOwner()) return;
	if (GetOwnerRole() != ROLE_Authority) return;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::SR_OnTakeDamage);
	OnDeathSignature.AddDynamic(this, &UHealthComponent::DefaultDeath);

}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UHealthComponent, Health);
}

void UHealthComponent::SR_Heal_Implementation(float HealAmount)
{
	if (GetOwnerRole() != ROLE_Authority || HealAmount <= 0.f || bIsDead) return;
	float OldHealth = Health;
	Health = FMath::Clamp(Health + HealAmount, 0.f, MaxHealth);
	float ActualHealed = Health - OldHealth;
	if (ActualHealed > 0.f) OnHealthChangedSignature.Broadcast(this, Health, ActualHealed);
}

void UHealthComponent::SR_OnTakeDamage_Implementation(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
													  AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage < 0.f || bIsDead) return;
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
	OnHealthChangedSignature.Broadcast(this, Health, -Damage);
	if (Health <= 0.f) { bIsDead = true; OnDeathSignature.Broadcast(); }
}

void UHealthComponent::DefaultDeath()
{
	if (!GetOwner() || GetOwnerRole() != ROLE_Authority) return;
	if (bAutoDestroyOnOwnerDeath) GetOwner()->Destroy();
}

