// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UMG/BaseWidgets/SlotBaseWidget.h"
#include "Character/InventoryComponent.h"
#include "InventoryBaseWidget.generated.h"

class AItem;
class UInventoryItemRowUserWidget;

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API UInventoryBaseWidget : public USlotBaseWidget
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
	UInventoryComponent* ReferencedInventory;

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Initialize Inventory Widget"))
	void InitWidget(UInventoryComponent* inventoryComponent);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory Events", meta = (FriendlyName = "Create Item Row"))
	UInventoryItemRowUserWidget* CreateItemRow(TSubclassOf<AItem> itemClass, UInventoryComponent* inventory);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory Events", meta = (FriendlyName = "On Item Added"))
	void OnItemAdded(TSubclassOf<AItem> itemClass, FItemClassCollection collection, AItem* added, UInventoryItemRowUserWidget* widget);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory Events", meta = (FriendlyName = "On Item Removed"))
	void OnItemRemoved(TSubclassOf<AItem> itemClass, FItemClassCollection collection, AItem* removed, UInventoryItemRowUserWidget* widget);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	void OnItemAdded_Internal(TSubclassOf<AItem> itemClass, FItemClassCollection itemClassCollection, AItem* added);

	UFUNCTION()
	void OnItemRemoved_Internal(TSubclassOf<AItem> itemClass, FItemClassCollection itemClassCollection, AItem* removed);

protected:

	TMap<TSubclassOf<AItem>, UInventoryItemRowUserWidget*> ItemClassRowsMap;

	void SetupItemRows();
};
