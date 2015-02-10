// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"
#include "InventorySlot.h"


AInventorySlot::AInventorySlot(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	SlotItemClasses.Empty();
	SlotItemClasses.Add(AInventoryItem::StaticClass());
}

void AInventorySlot::ReceiveDestroyed()
{
	if (OwningComponent != NULL)
	{
		OwningComponent->RemoveSlot(this, AInventorySlot::StaticClass());
	}
	DestroyItem();
	AActor::ReceiveDestroyed();
}
