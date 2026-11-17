// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/UI/CurrentPossession.h"

#include "Actors/Items/Item.h"
#include "Characters/BasePlayer.h"
#include "Components/Image.h"

void UCurrentPossession::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!DefaultIcon) return;
	ItemIcon->SetBrushFromTexture(DefaultIcon);
	
}

void UCurrentPossession::NativeDestruct()
{
	Super::NativeDestruct();
}

void UCurrentPossession::InitCurrentPossession(ABasePlayer* PlayerRef)
{
	if (!PlayerRef) return;
	CachedBasePlayer = PlayerRef;
	CachedBasePlayer->OnCurrentItemChangedSignature.AddDynamic(this, &UCurrentPossession::OnCurrentItemChanged);
	OnCurrentItemChanged();	
}

void UCurrentPossession::OnCurrentItemChanged()
{
	if (!(CachedBasePlayer && CachedBasePlayer->CurrentItem && CachedBasePlayer->CurrentItem->ItemIcon))
	{
		ItemIcon->SetBrushFromTexture(DefaultIcon);	
		return;
	}
	ItemIcon->SetBrushFromTexture(CachedBasePlayer->CurrentItem->ItemIcon);
}
