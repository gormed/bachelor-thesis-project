

#pragma once

#include "Object.h"
#include "Items/WeaponItem.h"
#include "WeaponSlot.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API AWeaponSlot : public AItemSlot
{
	GENERATED_UCLASS_BODY()

	/// All types of weapons that fit into this slot
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class|Types")
	TArray<TEnumAsByte<EWeaponType::Type>> WeaponTypes;

	//////////////////////////////////////////////////////////////////////////

	/// Additionally to 'UItemSlot::Fits' checks if the item type is contained by 'WeaponTypes'
	virtual bool Fits(class AItem* item) const override;

	/// Drops the item on destory and removes the item from the slot component
	virtual void ReceiveDestroyed() override;
};
