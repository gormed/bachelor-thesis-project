// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"
#include "ArmorSlot.h"
#include "ItemSlot.h"


AArmorSlot::AArmorSlot(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	SlotItemClasses.Empty();
	SlotItemClasses.Add(AArmorItem::StaticClass());
}

bool AArmorSlot::Fits(AItem* item) const
{
	AArmorItem* armor = Cast<AArmorItem>(item);
	if (armor == NULL)
	{
		UE_LOG(LogCaede, Error, TEXT("The armor to check for fit the slot is NULL or not a 'ArmorItem'!"));
		return false;
	}
	return SlotArmorTypes.Contains(armor->ArmorType) && AItemSlot::Fits(item);
}

void AArmorSlot::ReceiveDestroyed()
{
	if (OwningComponent != NULL)
	{
		OwningComponent->RemoveSlot(this, AArmorSlot::StaticClass());
	}
	DestroyItem();
	AActor::ReceiveDestroyed();
}
