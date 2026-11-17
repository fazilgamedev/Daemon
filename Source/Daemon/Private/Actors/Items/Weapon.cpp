// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/Weapon.h"

#include "Controllers/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Characters/BasePlayer.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::Use_Implementation(ABasePlayer* User)
{
	Super::Use_Implementation(User);
	
	if (!User) return;
	UseDelegate.BindUObject(this, &AWeapon::LineTrace, User);
	switch (WeaponType)
	{
	case EWeaponType::None:
		break;
	case EWeaponType::Melee:
		// for now
		break;
	case EWeaponType::Projectile:
		switch (FireSelect)
		{
		case EFireSelect::None:
			break;
		case EFireSelect::SemiAuto:
			GetWorldTimerManager().SetTimer(UseHandle, UseDelegate, 1.f / FireRate, false);
			break;
		case EFireSelect::FullAuto:
			GetWorldTimerManager().SetTimer(UseHandle, UseDelegate, 1.f / FireRate, true);
			break;
		}
		break;
	}
}

void AWeapon::StopUse_Implementation()
{
	Super::StopUse_Implementation();
	
	GetWorldTimerManager().ClearTimer(UseHandle);
}

void AWeapon::LineTrace(ABasePlayer* User)
{
	FVector S = User->Camera->GetComponentLocation();
	FVector E = S + Range * User->GetBaseAimRotation().Vector();
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(User);
	if (bTraceDebug) DrawDebugLine(GetWorld(), S, E, FColor::Red, false, 2.f, 0, 1.f);
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, S, E, ECC_Visibility, Params);
	if (bHit)
	{
		AActor* HitActor = OutHit.GetActor();
		if (!HitActor) return;
		UE_LOG(LogTemp, Warning, TEXT("Name of the hit actor is %s"), *OutHit.GetActor()->GetName());
		UGameplayStatics::ApplyDamage(HitActor, Damage, User->PC_Ref, User, UDamageType::StaticClass());
		MC_Fire(OutHit.ImpactPoint, OutHit.ImpactNormal.Rotation());
	}
}

void AWeapon::MC_Fire_Implementation(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (!HitEfx && GetWorld()) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEfx, SpawnLocation, SpawnRotation);
}

