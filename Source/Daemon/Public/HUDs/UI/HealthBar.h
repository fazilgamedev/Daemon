// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UHealthComponent;
class UProgressBar;

/**
 * 
 */
UCLASS()
class DAEMON_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
private:
	
	UPROPERTY()
	UHealthComponent* CachedHealthComp;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	virtual void NativeDestruct() override;
	
public:
	
	UFUNCTION()
	void InitHealthBar(UHealthComponent* HealthComponent);
	
	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(UHealthComponent* HealthComponent, float Health, float HealthDelta);
	

	
	
};
