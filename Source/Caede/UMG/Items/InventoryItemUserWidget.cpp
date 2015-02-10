// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"
#include "InventoryItemUserWidget.h"


UInventoryItemUserWidget::UInventoryItemUserWidget(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

void UInventoryItemUserWidget::InitWidget(AInventoryItem* item)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("Item for %s is invalid!"), *GetName());
		return;
	}
	ReferencedItem = item;

	InventoryWidget = CreateInventoryWidget(item->ItemInventory);
}


