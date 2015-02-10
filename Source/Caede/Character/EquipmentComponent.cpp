

#include "Caede.h"

#include "Character/InventoryComponent.h"
#include "Character/CaedeCharacter.h"
#include "Character/Slots/WeaponSlot.h"
#include "Character/Slots/ArmorSlot.h"
#include "Character/Slots/InventorySlot.h"
#include "Character/Slots/ItemSlot.h"
#include "Character/InteractionComponent.h"
#include "Items/BasicItem.h"
#include "Items/WeaponItem.h"
#include "Items/HerbItem.h"
#include "Items/QuestItem.h"
#include "Items/InventoryItem.h"

#include "Character/EquipmentComponent.h"

UEquipmentComponent::UEquipmentComponent(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

	CurrentNavigationMode = EInventoryMode::Inventory;
	CurrentNavigationItem = NULL;
	CurrentNavigationInventory = NULL;
	CurrentNavigationSlot = NULL;

	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostPhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

}

void UEquipmentComponent::BeginPlay()
{

	// Initially set the selection for inventory navigation

	CurrentNavigationMode = EInventoryMode::Inventory;

}

void UEquipmentComponent::BeginDestroy()
{
	UActorComponent::BeginDestroy();
}

//////////////////////////////////////////////////////////////////////////

template<class T>
bool UEquipmentComponent::CheckSlot(AItemSlot* slot, TSubclassOf<AItemSlot> baseType, T*& outType)
{
	if (slot == NULL)
	{
		UE_LOG(LogCaedeEquipment, Error, TEXT("The slot to equip to is NULL!"));
		outType = NULL;
		return false;
	}
	if (baseType == NULL)
	{
		UE_LOG(LogCaedeEquipment, Error, TEXT("The base class to compare is invalid!"));
		outType = NULL;
		return false;
	}
	T* slotType = Cast<T>(slot);
	if (slotType == NULL)
	{
		UE_LOG(LogCaedeEquipment, Error, TEXT("The slot is not of the required type!"));
		outType = NULL;
		return false;
	}
	outType = slotType;
	return OwningCharacter->Slots->ContainsSlot(slot, baseType);
}

bool UEquipmentComponent::CheckItem(AItem* item)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeEquipment, Error, TEXT("The item to equip is NULL!"));
		return false;
	}
	return true;
}

bool UEquipmentComponent::CheckSlots(TSubclassOf<AItemSlot> baseType)
{
	if (baseType == NULL)
	{
		UE_LOG(LogCaedeEquipment, Error, TEXT("The base class to compare is invalid!"));
		return false;
	}
	if (OwningCharacter->Slots == NULL)
	{
		UE_LOG(LogCaedeEquipment, Error, TEXT("Slot Component is invalid!"));
		return false;
	}
	if (OwningCharacter->Slots->SlotMap.Num() == 0)
	{
		UE_LOG(LogCaedeEquipment, Warning, TEXT("There is no item slot to equip to!"));
		return false;
	}
	if (!OwningCharacter->Slots->SlotMap.Contains(baseType))
	{
		UE_LOG(LogCaedeEquipment, Warning, TEXT("There is no item slot with the base type (%s) to equip to!"), *baseType->GetName());
		return false;
	}
	if (OwningCharacter->Slots->SlotMap[baseType].SlotIntstances.Num() == 0)
	{
		UE_LOG(LogCaedeEquipment, Warning, TEXT("There is no item for the base type (%s) to equip to!"), *baseType->GetName());
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool UEquipmentComponent::FindInventoryForItem(AItem* item, UInventoryComponent*& inventory)
{
	if (!CheckItem(item))
	{
		inventory = NULL;
		return false;
	}
	if (!CheckSlots(AInventorySlot::StaticClass()))
	{
		inventory = NULL;
		return false;
	}
	if (OwningCharacter->Slots->SlotMap.Contains(AInventorySlot::StaticClass()))
	{
		for (AItemSlot* slot : OwningCharacter->Slots->SlotMap[AInventorySlot::StaticClass()].SlotIntstances)
		{
			AInventorySlot* inventorySlot;
			if (CheckSlot<AInventorySlot>(slot, AInventorySlot::StaticClass(), inventorySlot))
			{
				AInventoryItem* inventoryItem = Cast<AInventoryItem>(inventorySlot->SlotItem);
				if (inventoryItem != NULL && inventoryItem->ItemInventory->FitsItem(item))
				{
					inventory = inventoryItem->ItemInventory;
					return true;
				}
			}
		}
	}
	inventory = NULL;
	return false;
}

bool UEquipmentComponent::IsInInventory(AItem* item, UInventoryComponent*& inventory)
{
	if (!CheckItem(item))
	{
		inventory = NULL;
		return false;
	}
	if (!CheckSlots(AInventorySlot::StaticClass()))
	{
		inventory = NULL;
		return false;
	}
	if (OwningCharacter->Slots->SlotMap.Contains(AInventorySlot::StaticClass()))
	{
		for (AItemSlot* slot : OwningCharacter->Slots->SlotMap[AInventorySlot::StaticClass()].SlotIntstances)
		{
			AInventorySlot* inventorySlot;
			if (CheckSlot<AInventorySlot>(slot, AInventorySlot::StaticClass(), inventorySlot))
			{
				AInventoryItem* inventoryItem = Cast<AInventoryItem>(inventorySlot->SlotItem);
				if (inventoryItem != NULL && inventoryItem->ItemInventory->ContainsItem(item))
				{
					inventory = inventoryItem->ItemInventory;
					return true;
				}
			}
		}
	}
	inventory = NULL;
	return false;
}

//////////////////////////////////////////////////////////////////////////

bool UEquipmentComponent::CanEquipWeapon(AWeaponItem* weapon)
{
	AItemSlot* slot;
	return FindWeaponSlot(weapon, slot) && slot != NULL;
}

bool UEquipmentComponent::FindWeaponSlot(AWeaponItem* weapon, AItemSlot*& toSlot)
{
	if (!CheckItem(weapon) || !CheckSlots(AWeaponSlot::StaticClass()))
	{
		return false;
	}
	// find a free slot
	return OwningCharacter->Slots->FindSlotForItem(weapon, AWeaponSlot::StaticClass(), toSlot);
}

int32 UEquipmentComponent::EquipWeapon(AWeaponItem* weapon)
{
	AItemSlot* slot;
	int32 slotIndex = -1;

	if (FindWeaponSlot(weapon, slot) && slot != NULL && EquipWeaponToSlot(weapon, slot))
	{
		AWeaponSlot* weaponSlot = Cast<AWeaponSlot>(slot);
		if (weaponSlot != NULL)
		{
			int32 slotIndex = weaponSlot->SlotIndex;
			return slotIndex;
		}
	}

	return slotIndex;
}

bool UEquipmentComponent::EquipWeaponToSlot(AWeaponItem* weapon, AItemSlot* slot)
{
	AWeaponSlot* weaponSlot;
	if (!CheckItem(weapon) || !CheckSlot<AWeaponSlot>(slot, AWeaponSlot::StaticClass(), weaponSlot))
	{
		return false;
	}

	// do not equip the same weapon twice, and ensure the weapon fits...
	if (weaponSlot != NULL && weaponSlot->SlotItem != weapon && weaponSlot->Fits(weapon))
	{
		UnequipWeaponFromSlot(weaponSlot);
		// CurrentSelectedInventory = ArmorInventory;
		// CurrentInventoryMode = EInventoryMode::Armor;
		weaponSlot->Insert(weapon);
		// LinkWeaponSlot(weaponSlots[slot]);
		// ArmorInventory->PickupItem(armor);
		return true;
	}
	UE_LOG(LogCaedeEquipment, Error, TEXT("The slot to equip to won't use the same weapon twice or the weapon does not fit!"));
	return false;
}

bool UEquipmentComponent::UnequipWeapon(AWeaponItem* weapon)
{
	if (!CheckItem(weapon))
	{
		return false;
	}

	AItemSlot* weaponSlot;
	if (OwningCharacter->Slots->ContainsItem(weapon, AWeaponSlot::StaticClass(), weaponSlot))
	{
		return UnequipWeaponFromSlot(weaponSlot) != NULL;
	}
	UE_LOG(LogCaedeEquipment, Warning, TEXT("Weapon %s not found to unequip!"), *weapon->LocalizedItemName.ToString());
	return false;
}

AWeaponItem* UEquipmentComponent::UnequipWeaponFromSlot(AItemSlot* slot)
{
	AWeaponSlot* weaponSlot;
	if (!CheckSlot<AWeaponSlot>(slot, AWeaponSlot::StaticClass(), weaponSlot))
	{
		return NULL;
	}

	// only if there is sth to unequip
	if (weaponSlot != NULL)
	{
		if (weaponSlot->IsOccupied())
		{
			AWeaponItem* weapon = Cast<AWeaponItem>(weaponSlot->Remove());
			UE_LOG(LogCaedeEquipment, Log, TEXT("Weapon %s unequiped from slot %s!"), *weapon->InternalItemName, *weaponSlot->GetName());
			// ArmorInventory->DropItem(armor);
			return weapon;
		}
		UE_LOG(LogCaedeEquipment, Log, TEXT("The weapon slot %s to unequip is not occupied!"), *weaponSlot->GetName());
		return NULL;
	}
	UE_LOG(LogCaedeEquipment, Error, TEXT("The weapon slot %s to unequip seems invalid!"), *slot->GetName());
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

bool UEquipmentComponent::CanEquipArmor(AArmorItem* armor)
{
	AItemSlot* slot;
	return FindArmorSlot(armor, slot) && slot != NULL;
}

bool UEquipmentComponent::FindArmorSlot(AArmorItem* armor, AItemSlot*& toSlot)
{
	if (!CheckItem(armor) || !CheckSlots(AArmorSlot::StaticClass()))
	{
		return false;
	}
	// find a free slot
	return OwningCharacter->Slots->FindSlotForItem(armor, AArmorSlot::StaticClass(), toSlot);
}

bool UEquipmentComponent::EquipArmorToSlot(AArmorItem* armor, AItemSlot* slot)
{
	AArmorSlot* armorSlot;
	if (!CheckItem(armor) || !CheckSlot<AArmorSlot>(slot, AArmorSlot::StaticClass(), armorSlot))
	{
		return false;
	}

	// do not equip the same armor twice, and ensure the armor fits...
	if (armorSlot != NULL && armorSlot->SlotItem != armor && armorSlot->Fits(armor))
	{
		UnequipArmorFromSlot(armorSlot);
		// CurrentSelectedInventory = ArmorInventory;
		// CurrentInventoryMode = EInventoryMode::Armor;
		armorSlot->Insert(armor);
		// LinkWeaponSlot(weaponSlots[slot]);
		// ArmorInventory->PickupItem(armor);
		return true;
	}
	UE_LOG(LogCaedeEquipment, Error, TEXT("The slot to equip to won't use the same armor twice or the armor does not fit!"));
	return false;
}

bool UEquipmentComponent::EquipArmor(AArmorItem* armor)
{
	AItemSlot* slot;
	if (FindArmorSlot(armor, slot) && slot != NULL && EquipArmorToSlot(armor, slot))
	{
		return true;
	}
	return false;
}

AArmorItem* UEquipmentComponent::UnequipArmorFromSlot(AItemSlot* slot)
{
	AArmorSlot* armorSlot;
	if (!CheckSlot<AArmorSlot>(slot, AArmorSlot::StaticClass(), armorSlot))
	{
		return NULL;
	}

	// only if there is sth to unequip
	if (armorSlot != NULL && armorSlot->IsOccupied())
	{
		UE_LOG(LogCaedeEquipment, Log, TEXT("Armor %s unequiped from slot!"), *armorSlot->SlotItem->LocalizedItemName.ToString());
		AArmorItem* armor = Cast<AArmorItem>(armorSlot->Remove());
		// ArmorInventory->DropItem(armor);
		return armor;
	}
	UE_LOG(LogCaedeEquipment, Error, TEXT("The armor slot to unequip seems invalid!"));
	return NULL;
}


bool UEquipmentComponent::UnequipArmor(AArmorItem* armor)
{
	if (!CheckItem(armor))
	{
		return false;
	}
	AItemSlot* armorSlot;
	if (OwningCharacter->Slots->ContainsItem(armor, AArmorSlot::StaticClass(), armorSlot))
	{
		return UnequipArmorFromSlot(armorSlot) != NULL;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////


bool UEquipmentComponent::CanEquipInventory(AInventoryItem* inventory)
{
	AItemSlot* slot;
	return FindInventorySlot(inventory, slot) && slot != NULL;
}

bool UEquipmentComponent::FindInventorySlot(AInventoryItem* inventory, AItemSlot*& toSlot)
{
	if (!CheckItem(inventory) || !CheckSlots(AInventorySlot::StaticClass()))
	{
		return false;
	}
	// find a free slot
	return OwningCharacter->Slots->FindSlotForItem(inventory, AInventorySlot::StaticClass(), toSlot);
}

bool UEquipmentComponent::EquipInventoryToSlot(AInventoryItem* inventory, AItemSlot* slot)
{
	AInventorySlot* inventorySlot;
	if (!CheckItem(inventory) || !CheckSlot<AInventorySlot>(slot, AInventorySlot::StaticClass(), inventorySlot))
	{
		return false;
	}

	// do not equip the same inventory twice, and ensure the inventory fits...
	if (inventorySlot != NULL && inventorySlot->SlotItem != inventory && inventorySlot->Fits(inventory))
	{
		UnequipInventoryFromSlot(inventorySlot);
		// CurrentSelectedInventory = ArmorInventory;
		// CurrentInventoryMode = EInventoryMode::Armor;
		inventorySlot->Insert(inventory);
		// LinkWeaponSlot(weaponSlots[slot]);
		// ArmorInventory->PickupItem(armor);
		return true;
	}
	UE_LOG(LogCaedeEquipment, Error, TEXT("The slot to equip to won't use the same inventory twice or the inventory does not fit!"));
	return false;
}

bool UEquipmentComponent::EquipInventory(AInventoryItem* inventory)
{
	AItemSlot* slot;
	if (FindInventorySlot(inventory, slot) && slot != NULL && EquipInventoryToSlot(inventory, slot))
	{
		return true;
	}
	return false;
}

AInventoryItem* UEquipmentComponent::UnequipInventoryFromSlot(AItemSlot* slot)
{
	AInventorySlot* inventorySlot;
	if (!CheckSlot<AInventorySlot>(slot, AInventorySlot::StaticClass(), inventorySlot))
	{
		return NULL;
	}

	// only if there is sth to unequip
	if (inventorySlot != NULL && inventorySlot->IsOccupied())
	{
		AInventoryItem* inventory = Cast<AInventoryItem>(inventorySlot->Remove());
		UE_LOG(LogCaedeEquipment, Log, TEXT("Inventory %s unequiped from slot %s!"), *inventory->InternalItemName, *slot->GetName());
		// ArmorInventory->DropItem(armor);
		return inventory;
	}
	UE_LOG(LogCaedeEquipment, Error, TEXT("The inventory slot to unequip seems invalid!"));
	return NULL;
}

bool UEquipmentComponent::UnequipInventory(AInventoryItem* inventory)
{
	if (!CheckItem(inventory))
	{
		return false;
	}

	AItemSlot* inventorySlot;
	if (OwningCharacter->Slots->ContainsItem(inventory, AInventorySlot::StaticClass(), inventorySlot))
	{
		return UnequipInventoryFromSlot(inventorySlot) != NULL;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////

bool UEquipmentComponent::Navigate(UUserWidget* instigator, TEnumAsByte<EInventoryMode::Type> mode, AItemSlot* slot, UInventoryComponent* inventory, AItem* item)
{

	FNavigationSelection oldSelection;
	FNavigationSelection newSelection;
	bool changed = false;

	oldSelection.NavigaionInventory = CurrentNavigationInventory;
	oldSelection.NavigationSlot = CurrentNavigationSlot;
	oldSelection.NavigationMode = CurrentNavigationMode;
	oldSelection.NavigationItem = CurrentNavigationItem;

	oldSelection.SlotChanged = false;
	newSelection.SlotChanged = false;
	oldSelection.InventoryChanged = false;
	newSelection.InventoryChanged = false;
	oldSelection.ItemChanged = false;
	newSelection.ItemChanged = false;

	switch (mode)
	{
		// Handle Invalid selection
	case EInventoryMode::Invalid:
	case EInventoryMode::Count:
		UE_LOG(LogCaedeEquipment, Error, TEXT("Invalid InventoryMode selected!"));
		return false;
		// otherwise set the correct new mode
	default:
		CurrentNavigationMode = mode;
		break;
	}

	// check if slot changed
	if (slot != NULL && slot != CurrentNavigationSlot)
	{
		CurrentNavigationSlot = slot;
		oldSelection.SlotChanged = true;
		newSelection.SlotChanged = true;
		changed = true;
	}

	// check if inventory changed
	if (inventory != NULL && inventory != CurrentNavigationInventory)
	{
		CurrentNavigationInventory = inventory;
		oldSelection.InventoryChanged = true;
		newSelection.InventoryChanged = true;
		changed = true;
	}

	// check if item changed
	if (item != NULL && item != CurrentNavigationItem)
	{
		CurrentNavigationItem = item;
		oldSelection.ItemChanged = true;
		newSelection.ItemChanged = true;
		changed = true;
	}

	// if changes occurred
	if (changed)
	{
		// collect and broadcast the changes
		newSelection.NavigaionInventory = CurrentNavigationInventory;
		newSelection.NavigationSlot = CurrentNavigationSlot;
		newSelection.NavigationMode = CurrentNavigationMode;
		newSelection.NavigationItem = CurrentNavigationItem;

		OnNavigationChanged.Broadcast(instigator, oldSelection, newSelection);
	}
	// return true if changes occurred, false otherwise
	return changed;
}

void UEquipmentComponent::ClearNavigation()
{
	CurrentNavigationItem = NULL;
	CurrentNavigationSlot = NULL;
	CurrentNavigationInventory = NULL;
	CurrentNavigationMode = EInventoryMode::Invalid;
}

//////////////////////////////////////////////////////////////////////////

AItem* UEquipmentComponent::SwitchDirectly(AItem* item, bool unselect)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeEquipment, Error, TEXT("Item to switch to is NULL!"));
		return NULL;
	}

	return DoSelection(item, unselect);
}

AItem* UEquipmentComponent::DoSelection(AItem* item, bool unselect)
{
	if (HandSelectedItem == item)
	{
		UE_LOG(LogCaedeEquipment, Warning, TEXT("Reselected the current 'HandSelectedItem' %s!"), *HandSelectedItem->InternalItemName);
		return HandSelectedItem;
	}
	switch (item->UsesHands)
	{
	case EHandSlot::LeftHand: // fall through
	case EHandSlot::RightHand: // fall through
	case EHandSlot::BothHands: // handle left, right and both hands equally
		if (unselect)
		{
			UnselectItem(true);
		}

		// Select current item in hand
		HandSelectedItem = item;
		HandSelectedInventory = item->CurrentInventory;
		HandSelectedSlot = item->CurrentItemSlot;

		HandSelectedItem->OnSelected(this);
		OnItemSelected.Broadcast(HandSelectedItem, HandSelectedItem->UsesHands);
		HandSelectedItem->OnIsSelected.Broadcast(OwningCharacter, HandSelectedItem->UsesHands);

		//CurrentNavigationInventory = HandSelectedInventory;

		UE_LOG(LogCaedeEquipment, Log, TEXT("Selected hand %d with item %s!"), (uint8)HandSelectedItem->UsesHands, *HandSelectedItem->LocalizedItemName.ToString());

		return HandSelectedItem;
	case EHandSlot::NoHands:
		// Select a item without altering the current hand items!
		item->OnSelected(this);
		OnItemSelected.Broadcast(item, item->UsesHands);
		item->OnIsSelected.Broadcast(OwningCharacter, item->UsesHands);

		UE_LOG(LogCaedeEquipment, Log, TEXT("Selected without hands %d with item %s!"), (uint8)item->UsesHands, *item->LocalizedItemName.ToString());

		return item;
	default:

		UE_LOG(LogCaedeEquipment, Error, TEXT("Selected invalid slot %d!"), (uint8)HandSelectedItem->UsesHands);
		return NULL;
	}
}

AItem* UEquipmentComponent::UnselectItem(bool fireOnUselectItemEvent)
{
	return DoUnselection(fireOnUselectItemEvent);
}

AItem* UEquipmentComponent::DoUnselection(bool fireOnUnselectItemEvent)
{
	AItem* unselected;
	if (HandSelectedItem == NULL)
	{
		UE_LOG(LogCaedeEquipment, Warning, TEXT("Nothing to unselect in hands!"));
		return NULL;
	}
	unselected = HandSelectedItem;

	UE_LOG(LogCaedeEquipment, Log, TEXT("Unselected left hand %d with item %s!"), (uint8)HandSelectedItem->UsesHands, *HandSelectedItem->LocalizedItemName.ToString());

	HandSelectedSlot = NULL;
	HandSelectedInventory = NULL;
	HandSelectedItem = NULL;

	unselected->OnUnselected(this);

	if (fireOnUnselectItemEvent)
	{
		OnItemUnselected.Broadcast(unselected, unselected->UsesHands);
		unselected->OnIsUnselected.Broadcast(OwningCharacter, unselected->UsesHands);
	}

	return unselected;
}

AItem* UEquipmentComponent::SelectItem(AItem* item)
{
	return SelectItemSilent(item, true);
}

AItem* UEquipmentComponent::SelectItemSilent(AItem* item, bool unselect)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeEquipment, Error, TEXT("Item to switch to is NULL!"));
		return NULL;
	}

	return SwitchDirectly(item, unselect);
}

//////////////////////////////////////////////////////////////////////////

void UEquipmentComponent::LinkWeaponSlot(AWeaponSlot* from)
{
	TArray<AItemSlot*> weaponSlots = OwningCharacter->Slots->GetSlots(AWeaponSlot::StaticClass());
	for (AItemSlot* to : weaponSlots)
	{
		if (to->IsOccupied() && from->IsOccupied() && to != from)
		{
			AWeaponItem* fromWeapon = Cast<AWeaponItem>(from->SlotItem);
			AWeaponItem* toWeapon = Cast<AWeaponItem>(to->SlotItem);
			// link one hand and shield
			if (fromWeapon->WeaponType == EWeaponType::OneHand
				&& toWeapon->WeaponType == EWeaponType::Shield)
			{
				from->Link(to);
				to->Link(from);

				continue;
			}
			else if (fromWeapon->WeaponType == EWeaponType::Shield
				&& toWeapon->WeaponType == EWeaponType::OneHand)
			{
				from->Link(to);
				to->Link(from);
				continue;
			}
			// TODO: link bow and arrows
		}
	}
}

void UEquipmentComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
