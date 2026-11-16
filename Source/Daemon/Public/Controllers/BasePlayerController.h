// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UGameHUD;

/**
 * 
 */
UCLASS()
class DAEMON_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameHUD> GameHUDClass;
	
	virtual void BeginPlay() override;
	
};
