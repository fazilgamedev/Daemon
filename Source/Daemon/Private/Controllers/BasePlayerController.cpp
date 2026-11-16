// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/BasePlayerController.h"

#include "Characters/BasePlayer.h"
#include "HUDs/UI/GameHUD.h"
#include "HUDs/UI/HealthBar.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController()) return;
	ABasePlayer* PlayerRef = Cast<ABasePlayer>(GetPawn());
	if (!PlayerRef) return;
	if (!GameHUDClass) return;
	UGameHUD* GameHUD = CreateWidget<UGameHUD>(this, GameHUDClass);
	if (!GameHUD) return;
	GameHUD->AddToViewport();
	
	// For HealthBar
	if (PlayerRef->HealthComponent) GameHUD->HealthBar->InitHealthBar(PlayerRef->HealthComponent);
}
