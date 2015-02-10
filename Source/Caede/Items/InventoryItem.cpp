// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"
#include "InventoryItem.h"


AInventoryItem::AInventoryItem(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	AItem::UsesHands = EHandSlot::NoHands;
	AItem::CanBeDropped = true;
	AItem::CanBeEquipped = true;
	AItem::CanBePickedUp = true;
	AItem::CanBeUsed = false;

	ItemInventory = PCIP.CreateDefaultSubobject<class UInventoryComponent>(this, TEXT("Item Inventory"));

	//ItemInventory->OwningItem = this;
}

//////////////////////////////////////////////////////////////////////////

float AInventoryItem::ActualWeight_Internal()
{
	return ItemInventory->CurrentInventoryWeight;
}

float AInventoryItem::PercentageAmount_Internal()
{
	if (ItemInventory->HasWeight)
	{
		return ItemInventory->CurrentInventoryWeight / ItemInventory->MaxInventoryWeight;
	}
	else if (ItemInventory->HasCount)
	{
		return ItemInventory->CurrentInventoryCount / ItemInventory->MaxInventoryCount;
	}
	else
	{
		return 1.f;
	}
}

bool AInventoryItem::IsEmpty_Internal()
{
	return ItemInventory->IsEmpty();
}


