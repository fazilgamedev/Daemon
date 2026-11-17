// Fill out your copyright notice in the Description page of Project Settings.


#include "Daemon/Public/Characters/BasePlayer.h"

#include "Components/PostProcessComponent.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ActorComponents/Health/HealthComponent.h"
#include "Actors/Interactables/Interactable.h"
#include "Actors/Items/Item.h"
#include "AnimInstances/MannequinAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Controllers/BasePlayerController.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ABasePlayer::ABasePlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh(), FName("CamSocket"));
	CameraBoom->TargetArmLength = 150.f;
		
	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(CameraBoom, FName("SpringEndPoint"));
	Camera->SetFieldOfView(120.f);
	// Camera->bUsePawnControlRotation = true;
	// Camera->SetupAttachment(GetMesh(), FName("head"));

	PPComp = CreateDefaultSubobject<UPostProcessComponent>(FName("PPComp"));
	PPComp->bUnbound = false;
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
	HealthComponent->OnDeathSignature.AddDynamic(this, &ABasePlayer::SR_OnDeath);
	
	CurrentItem = nullptr;
	
}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	PC_Ref = Cast<ABasePlayerController>(GetController());
	if (!PC_Ref) return;
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC_Ref->GetLocalPlayer());
	if (!Subsystem && !IMC_Default) return;
	Subsystem->AddMappingContext(IMC_Default, 0);
	
	MannyAnimRef = Cast<UMannequinAnimInstance>(GetMesh()->GetAnimInstance());
	
}

// Called every frame
void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC) return;
	EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &ABasePlayer::Jump);
	EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ABasePlayer::StopJumping);
	EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABasePlayer::Move);
	EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABasePlayer::Look);
	EIC->BindAction(IA_Interact, ETriggerEvent::Started, this, &ABasePlayer::SR_Interact);
	EIC->BindAction(IA_DropCurrent, ETriggerEvent::Triggered, this, &ABasePlayer::SR_DropCurrent);
	EIC->BindAction(IA_UseItem, ETriggerEvent::Started, this, &ABasePlayer::SR_Use);
	EIC->BindAction(IA_UseItem, ETriggerEvent::Completed, this, &ABasePlayer::SR_StopUse);
}

void ABasePlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABasePlayer, CurrentItem);
}

void ABasePlayer::Move(const FInputActionValue& Value)
{
	const FVector2D Vec = Value.Get<FVector2D>();
	const FRotator Yaw = FRotator(0.f, PC_Ref->GetControlRotation().Yaw, 0.f);
	const FVector F = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	const FVector R = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
	AddMovementInput(F, Vec.Y);
	AddMovementInput(R, Vec.X);	
}

void ABasePlayer::Look(const FInputActionValue& Value)
{
	const FVector2D Vec = Value.Get<FVector2D>();
	AddControllerPitchInput(-Vec.Y * VSen);
	AddControllerYawInput(Vec.X * HSen);
}

void ABasePlayer::OnRep_CurrentItem()
{
	OnCurrentItemChangedSignature.Broadcast();
}

void ABasePlayer::SR_StopUse_Implementation()
{
	if (!CurrentItem) return;
	CurrentItem->StopUse();
}

void ABasePlayer::SR_Use_Implementation()
{
	if (!CurrentItem) return;
	CurrentItem->Use(this);
}


void ABasePlayer::SR_OnDeath_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Dying"));
	HealthComponent->Health = 0.f;
	Destroy();
}

void ABasePlayer::SR_Interact_Implementation()
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors, AInteractable::StaticClass());
	if (Actors.Num() < 1 || !IsValid(Actors[0])) return;
	AInteractable* New= Cast<AInteractable>(Actors[0]);
	New->OnInteract(this);
}

void ABasePlayer::SR_DropCurrent_Implementation()
{
	if (CurrentItem) { 
		CurrentItem->OnDetach(this, 500.f);
		CurrentItem = nullptr;
		if (HasAuthority()) OnRep_CurrentItem();
	}
}
