// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UMG/BaseWidgets/SlotBaseWidget.h"
#include "InventoryItemUserWidget.generated.h"

class AItem;
class AInventoryItem;

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API UInventoryItemUserWidget : public USlotBaseWidget
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory Item")
	AInventoryItem* ReferencedItem;

	UFUNCTION(BlueprintCallable, Category = "Inventory Item", meta = (FriendlyName = "Initialize Item Widget"))
	void InitWidget(AInventoryItem* item);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory Widget")
	UInventoryBaseWidget* InventoryWidget;

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory Item Events", meta = (FriendlyName = "Create Inventory Widget"))
	UInventoryBaseWidget* CreateInventoryWidget(UInventoryComponent* inventory);
};
