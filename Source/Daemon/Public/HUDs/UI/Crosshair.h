// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Crosshair.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class DAEMON_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UImage* U;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UImage* D;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UImage* L;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UImage* R;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UImage* C;
	
	
	
};
