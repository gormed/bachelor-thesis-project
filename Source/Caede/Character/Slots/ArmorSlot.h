// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Items/ArmorItem.h"
#include "ArmorSlot.generated.h"

class AItem;
class AArmorItem;

/**
*
*/
UCLASS(BlueprintType)
class CAEDE_API AArmorSlot : public AItemSlot
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////

	/// All types of armor that fit into this slot
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Armor")
	TArray<TEnumAsByte<EArmorType::Type>> SlotArmorTypes;

	//////////////////////////////////////////////////////////////////////////

	/// Additionally to 'UItemSlot::Fits' checks if the item type is contained by 'ArmorTypes'
	virtual bool Fits(AItem* item) const override;

	/// Drops the item on destroy and removes the item from the slot component
	virtual void ReceiveDestroyed() override;
};
