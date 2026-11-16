// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

class UCrosshair;
class UHealthBar;
/**
 * 
 */
UCLASS()
class DAEMON_API UGameHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UHealthBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UCrosshair* Crosshair;
	
};
