// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"

#include "UMG/Items/InventoryItemRowUserWidget.h"

#include "InventoryBaseWidget.h"


UInventoryBaseWidget::UInventoryBaseWidget(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

void UInventoryBaseWidget::InitWidget(UInventoryComponent* inventoryComponent)
{
	if (inventoryComponent == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("Inventory component for %s is invalid!"), *GetName());
		return;
	}
	ReferencedInventory = inventoryComponent;

	SetupItemRows();

	ReferencedInventory->OnItemAdded.AddUniqueDynamic(this, &UInventoryBaseWidget::OnItemAdded_Internal);
	ReferencedInventory->OnItemRemoved.AddUniqueDynamic(this, &UInventoryBaseWidget::OnItemRemoved_Internal);
}

void UInventoryBaseWidget::OnItemAdded_Internal(TSubclassOf<AItem> itemClass, FItemClassCollection itemClassCollection, AItem* added)
{
	if (added == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("UInventoryBaseWidget: Invalid item to add!"));
		return;
	}
	if (!ItemClassRowsMap.Contains(itemClass))
	{
		UInventoryItemRowUserWidget* widget = CreateItemRow(itemClass, ReferencedInventory);
		if (widget != NULL)
		{
			ItemClassRowsMap.Add(itemClass, widget);
			OnItemAdded(itemClass, itemClassCollection, added, widget);
			widget->OnItemAdded(itemClass, added);
		}
	}
	else
	{
		UInventoryItemRowUserWidget* widget = ItemClassRowsMap[itemClass];
		if (widget != NULL)
		{
			OnItemAdded(itemClass, itemClassCollection, added, widget);
			widget->OnItemAdded(itemClass, added);
		}
	}
}

void UInventoryBaseWidget::OnItemRemoved_Internal(TSubclassOf<AItem> itemClass, FItemClassCollection itemClassCollection, AItem* removed)
{
	if (removed == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("UInventoryBaseWidget: Invalid item to remove!"));
		return;
	}
	if (itemClassCollection.ItemCollection.Num() == 0 
		&& ItemClassRowsMap.Contains(itemClass))
	{
		UInventoryItemRowUserWidget* widget = ItemClassRowsMap[itemClass];
		ItemClassRowsMap.Remove(itemClass);
		OnItemRemoved(itemClass, itemClassCollection, removed, widget);
		widget->RemoveFromParent();
		//widget->OnItemRemoved(itemClass, removed);
	}
	else if (ItemClassRowsMap.Contains(itemClass))
	{
		UInventoryItemRowUserWidget* widget = ItemClassRowsMap[itemClass];
		OnItemRemoved(itemClass, itemClassCollection, removed, widget);
		widget->OnItemRemoved(itemClass, removed);
	}
}

void UInventoryBaseWidget::SetupItemRows()
{
	TArray<FItemClassCollection> itemCollections = ReferencedInventory->GetItemsSortedByType();
	UInventoryItemRowUserWidget* widget = NULL;

	ItemClassRowsMap.Empty(itemCollections.Num());
	for (FItemClassCollection collection : itemCollections)
	{
		widget = CreateItemRow(collection.ItemClass, ReferencedInventory);
		if (widget != NULL)
		{
			ItemClassRowsMap.Add(collection.ItemClass, widget);
		}
	}
}


