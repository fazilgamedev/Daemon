// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentPossession.generated.h"


class ABasePlayer;
class AItem;
class UImage;
/**
 * 
 */
UCLASS()
class DAEMON_API UCurrentPossession : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* DefaultIcon;
	
	UPROPERTY()
	ABasePlayer* CachedBasePlayer;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void InitCurrentPossession(ABasePlayer* PlayerRef);
	
	UFUNCTION()
	void OnCurrentItemChanged();
	
};
