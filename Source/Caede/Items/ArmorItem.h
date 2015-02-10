// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/Item.h"
#include "Items/WeaponItem.h"
#include "ArmorItem.generated.h"

UENUM(BlueprintType, Category = "Armor")
namespace EArmorType
{
	enum Type
	{
		Shoulder,
		Legs,
		Breast,
		Head,
		Hands,
		Shoes,
		Count,
		Invalid
	};
}

/**
 * The basic armor item, that can be equipped by a equipment component of a character
 */
UCLASS(BlueprintType)
class CAEDE_API AArmorItem : public AItem
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Armor")
	TEnumAsByte<EArmorType::Type> ArmorType;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Armor", meta = (FriendlyName = "Calculate Damage Reduction"))
	float CalculateDamageReduction(FWeaponHitDesc WeaponHitDesc, class AWeaponItem* Weapon);

};
