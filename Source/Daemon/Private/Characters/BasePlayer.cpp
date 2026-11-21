// Fill out your copyright notice in the Description page of Project Settings.


#include "Daemon/Public/Characters/BasePlayer.h"

#include "Components/CapsuleComponent.h"
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
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ABasePlayer::ABasePlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh(), FName("CamSocket"));
	CameraBoom->TargetArmLength = 0.f;
		
	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->SetFieldOfView(120.f);
	// Camera->bUsePawnControlRotation = true;
	// Camera->SetupAttachment(GetMesh(), FName("head"));

	// PPComp = CreateDefaultSubobject<UPostProcessComponent>(FName("PPComp"));
	// PPComp->bUnbound = false;
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
	HealthComponent->bAutoDestroyOnOwnerDeath = false;
	
	CurrentItem = nullptr;
	
	// OnAimStateChanged.AddDynamic(this, &ABasePlayer::OnADS);
	
	SR_StopSprint();
	
	DeathAnims.Init(nullptr, 3);
	
}

void ABasePlayer::OnRep_bIsAiming()
{
	if (!(CurrentItem && CurrentItem->ItemType == EItemType::Weapon)) return;
	OnAimStateChanged.Broadcast(bIsAiming);
}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority()) HealthComponent->OnDeathSignature.AddDynamic(this, &ABasePlayer::SR_OnDeath);
	
	PC_Ref = Cast<ABasePlayerController>(GetController());
	if (PC_Ref)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC_Ref->GetLocalPlayer());
		if (!Subsystem && !IMC_Default) return;
		Subsystem->AddMappingContext(IMC_Default, 0);
	}
	
	MannyAnimRef = Cast<UMannequinAnimInstance>(GetMesh()->GetAnimInstance());
	if (MannyAnimRef) UE_LOG(LogTemp, Warning, TEXT("AnimBP is Valid"));
	
}

// Called every frame
void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float Speed = GetVelocity().Size2D();
	float TargetFreq = 0.f;
	FVector TargetAmp = FVector::ZeroVector;
	if(Speed > 400.f){
		TargetFreq = 13.96f;
		TargetAmp = FVector(2.f, 2.f, 2.f);
	}else if(Speed > 3.f){
		TargetFreq = 10.47f;
		TargetAmp = FVector(.5f, .5f, .5f);
	}else{
		TargetFreq = 3.14;
		TargetAmp = FVector(.12f, .12f, .12f);
	}
	CurrentAmp = FMath::VInterpTo(CurrentAmp, TargetAmp, DeltaTime, 5.f);
	CurrentPhase += DeltaTime * TargetFreq;
	if(CurrentPhase > 6.28f) CurrentPhase -= 6.28f;
	WalkVector = FVector(FMath::Sin(CurrentPhase) * CurrentAmp.X, FMath::Cos(CurrentPhase) * CurrentAmp.Y, FMath::Sin(CurrentPhase * 2.f) * CurrentAmp.Z);
	
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
	EIC->BindAction(IA_Aim, ETriggerEvent::Triggered, this, &ABasePlayer::SR_ADS);
	EIC->BindAction(IA_Aim, ETriggerEvent::Completed, this, &ABasePlayer::SR_StopADS);
	EIC->BindAction(IA_Sprint, ETriggerEvent::Started, this, &ABasePlayer::SR_Sprint);
	EIC->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &ABasePlayer::SR_StopSprint);
}

void ABasePlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABasePlayer, CurrentItem);
	DOREPLIFETIME(ABasePlayer, bIsAiming);
}

void ABasePlayer::Move(const FInputActionValue& Value)
{
	const FVector2D Vec = Value.Get<FVector2D>();
	const FRotator Yaw = FRotator(0.f, PC_Ref->GetControlRotation().Yaw, 0.f);
	const FVector F = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	const FVector R = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
	AddMovementInput(F, Vec.Y);
	AddMovementInput(R, Vec.X);	
	if(MannyAnimRef) FrontMove = MannyAnimRef->AimAlpha > .5f ? Vec.X * .2f : Vec.X;
	else FrontMove = Vec.X;
	SideMove = Vec.Y * 2.f;
}

void ABasePlayer::Look(const FInputActionValue& Value)
{
	const FVector2D Vec = Value.Get<FVector2D>();
	AddControllerPitchInput(-Vec.Y * VSen);
	AddControllerYawInput(Vec.X * HSen);
}

void ABasePlayer::SR_StopSprint_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABasePlayer::SR_Sprint_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ABasePlayer::SR_StopADS_Implementation()
{
	if (!(CurrentItem && CurrentItem->ItemType == EItemType::Weapon)) return;
	bIsAiming = false;
	if (HasAuthority()) OnRep_bIsAiming(); 
}

void ABasePlayer::SR_ADS_Implementation()
{
	if (!(CurrentItem && CurrentItem->ItemType == EItemType::Weapon)) return;
	bIsAiming = true;
	if (HasAuthority()) OnRep_bIsAiming();
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
	SR_DropCurrent();
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (DeathAnims.Num() > 0) MC_OnDeath(FMath::RandRange(0, DeathAnims.Num() - 1));
	CT_OnDeath();
	FTimerHandle FDeathDestroyHandle;
	GetWorldTimerManager().SetTimer(FDeathDestroyHandle,    FTimerDelegate::CreateWeakLambda(this, [this](){ Destroy(); }), AfterDeathDestroyDelay, false);
}

void ABasePlayer::MC_OnDeath_Implementation(int32 AnimIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Dying with Animation"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (DeathAnims.IsValidIndex(AnimIndex)) GetMesh()->PlayAnimation(DeathAnims[AnimIndex], false);
}

void ABasePlayer::CT_OnDeath_Implementation()
{
	FInputModeUIOnly IMUIOnly;
	IMUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	if (IsLocallyControlled() && PC_Ref && PC_Ref->IsLocalController())
	{
		PC_Ref->SetInputMode(IMUIOnly);
		PC_Ref->bShowMouseCursor = true;
	}
	CameraBoom->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("root"));
	Camera->AttachToComponent(CameraBoom, FAttachmentTransformRules::SnapToTargetNotIncludingScale, USpringArmComponent::SocketName);
	CameraBoom->SetRelativeLocation(FVector::ZeroVector);
	Camera->SetRelativeLocation(FVector::ZeroVector);
	Camera->SetRelativeRotation(FRotator::ZeroRotator);
	CameraBoom->bUsePawnControlRotation = false;
	Camera->bUsePawnControlRotation = false;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	CameraBoom->TargetArmLength = 300.f;
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
