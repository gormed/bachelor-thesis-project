// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API AInventorySlot : public AItemSlot
{
	GENERATED_UCLASS_BODY()

	/// Drops the item on destroy and removes the item from the slot component
	virtual void ReceiveDestroyed() override;
};
