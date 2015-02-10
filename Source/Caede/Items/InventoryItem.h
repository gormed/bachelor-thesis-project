// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/Item.h"
#include "InventoryItem.generated.h"

/**
 * This item simply has an inventory...
 */
UCLASS(BlueprintType)
class CAEDE_API AInventoryItem : public AItem
{
	GENERATED_UCLASS_BODY()

	/// Inventory Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Inventory")
	class UInventoryComponent* ItemInventory;
	
protected:

	float ActualWeight_Internal() override;
	float PercentageAmount_Internal() override;
	bool IsEmpty_Internal() override;

};
