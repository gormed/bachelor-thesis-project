

#include "Caede.h"
#include "WeaponItem.h"
#include "Item.h"


AWeaponItem::AWeaponItem(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	CanBePickedUp = true;
	CanBeDropped = true;
	CanBeUsed = false;
	CanBeEquipped = false;
}


