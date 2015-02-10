// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"
#include "Character/InventoryComponent.h"
#include "InventoryItemRowUserWidget.h"


UInventoryItemRowUserWidget::UInventoryItemRowUserWidget(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

void UInventoryItemRowUserWidget::InitWidget(TSubclassOf<AItem> itemClass, UInventoryComponent* itemInventory)
{
	if (itemClass == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("Item class for %s is invalid!"), *GetName());
		return;
	}

	ReferencedItemClass = itemClass;
	ReferencedInventory = itemInventory;
}


