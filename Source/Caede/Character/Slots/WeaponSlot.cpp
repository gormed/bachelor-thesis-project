

#include "Caede.h"
#include "WeaponSlot.h"
#include "Items/WeaponItem.h"
#include "Character/Slots/ItemSlot.h"


AWeaponSlot::AWeaponSlot(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	SlotItemClasses.Empty();
	SlotItemClasses.Add(AWeaponItem::StaticClass());
}

bool AWeaponSlot::Fits(AItem* item) const
{
	AWeaponItem* weapon = Cast<AWeaponItem>(item);
	if (weapon == NULL)
	{
		UE_LOG(LogCaede, Error, TEXT("The weapon to check for fit the slot is NULL or not a 'WeaponItem'!"));
		return false;
	}
	return WeaponTypes.Contains(weapon->WeaponType) && AItemSlot::Fits(weapon);
}

void AWeaponSlot::ReceiveDestroyed()
{
	if (OwningComponent != NULL)
	{
		OwningComponent->RemoveSlot(this, AWeaponSlot::StaticClass());
	}
	DestroyItem();
	AActor::ReceiveDestroyed();
}
