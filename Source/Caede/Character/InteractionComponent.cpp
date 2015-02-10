

#include "Caede.h"
#include "InteractionComponent.h"
#include "Items/Item.h"
#include "Character/CaedeCharacter.h"
#include "Character/EquipmentComponent.h"
#include "Character/InventoryComponent.h"
#include "Items/WeaponItem.h"

UInteractionComponent::UInteractionComponent(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostPhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

}

bool UInteractionComponent::StartUseItem(AItem* item)
{
	AItem* currentInteraction;
	TEnumAsByte<EItemInteractionType::Type> interactionType;
	if (IsInteractingWithItem(currentInteraction, interactionType))
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("There is currently an item %s in interaction %d!"), *currentInteraction->InternalItemName, (uint8) interactionType);
		return false;
	}

	if (item == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to use is NULL!"));
		return false;
	}
	if (!item->CanBeUsed)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be used!"), *item->LocalizedItemName.ToString());
		return false;
	}
	CurrentUseItem = item;
	CurrentItemSlot = item->CurrentItemSlot;
	CurrentInventory = item->CurrentInventory;
	item->OnStartItemUse.Broadcast(OwningCharacter);
	return true;

}

bool UInteractionComponent::TickUseItem(float deltaTime)
{
	if (CurrentUseItem == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to use is NULL!"));
		return false;
	}
	if (!CurrentUseItem->CanBeUsed)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be used!"), *CurrentUseItem->LocalizedItemName.ToString());
		return false;
	}
	CurrentUseItem->OnTickItemUse.Broadcast(deltaTime, OwningCharacter);
	return true;
}

bool UInteractionComponent::EndUseItem(bool abort)
{
	if (CurrentUseItem == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to use is NULL!"));
		return false;
	}
	if (!CurrentUseItem->CanBeUsed)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be used!"), *CurrentUseItem->LocalizedItemName.ToString());
		return false;
	}
	if (!abort)
	{
		CurrentUseItem->OnEndItemUse.Broadcast(OwningCharacter);
		PerformUse(CurrentUseItem);
	}
	CurrentUseItem = NULL;
	CurrentItemSlot = NULL;
	CurrentInventory = NULL;
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool UInteractionComponent::CanStoreItem(AItem* item)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to store is NULL!"));
		return false;
	}
	if (!item->CanBeStored)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be stored!"), *item->LocalizedItemName.ToString());
		return false;
	}

	UInventoryComponent* inventory;
	AInventoryItem* inventoryItem = Cast<AInventoryItem>(item);
	AWeaponItem* weaponItem = Cast<AWeaponItem>(item);
	AArmorItem* armorItem = Cast<AArmorItem>(item);

	if (inventoryItem != NULL && OwningCharacter->Equipment->CanEquipInventory(inventoryItem))
	{
		return true;
	}

	else if (weaponItem != NULL && OwningCharacter->Equipment->CanEquipWeapon(weaponItem))
	{
		return true;
	}

	else if (armorItem != NULL && OwningCharacter->Equipment->CanEquipArmor(armorItem))
	{
		return true;
	}

	//if (item != NULL /*&& OwningCharacter->Equipment->CanEquipItem(basic)*/)
	//{
	//	//return OwningCharacter->Equipment->CanEquipItem(basic);
	//	return true;
	//}
	else if (item != NULL && OwningCharacter->Equipment->FindInventoryForItem(item, inventory) && inventory != NULL)
	{
		return true;
	}
	UE_LOG(LogCaedeInteraction, Log, TEXT("Cannot store item in inventory or slot!"));
	return false;
}

bool UInteractionComponent::StartStoreItem(class AItem* item)
{
	AItem* currentInteraction;
	TEnumAsByte<EItemInteractionType::Type> interactionType;
	if (IsInteractingWithItem(currentInteraction, interactionType))
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("There is currently an item %s in interaction %d!"), *currentInteraction->InternalItemName, (uint8)interactionType);
		return false;
	}

	if (item == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to store is NULL!"));
		return false;
	}
	if (!item->CanBeStored)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be stored!"), *item->LocalizedItemName.ToString());
		return false;
	}

	UInventoryComponent* inventory;
	AItemSlot* slot;
	bool canStore = false;

	AInventoryItem* inventoryItem = Cast<AInventoryItem>(item);
	AWeaponItem* weaponItem = Cast<AWeaponItem>(item);
	AArmorItem* armorItem = Cast<AArmorItem>(item);

	if (inventoryItem != NULL && OwningCharacter->Equipment->FindInventorySlot(inventoryItem, slot))
	{
		CurrentItemSlot = slot;
		CurrentInventory = NULL;
		canStore = true;
	}

	else if (weaponItem != NULL && OwningCharacter->Equipment->FindWeaponSlot(weaponItem, slot))
	{
		CurrentItemSlot = slot;
		CurrentInventory = NULL;
		canStore = true;
	}

	else if (armorItem != NULL && OwningCharacter->Equipment->FindArmorSlot(armorItem, slot))
	{
		CurrentItemSlot = slot;
		CurrentInventory = NULL;
		canStore = true;
	}

	//if (item != NULL /*&& OwningCharacter->Equipment->CanEquipItem(basic)*/)
	//{
	//	//return OwningCharacter->Equipment->CanEquipItem(basic);
	//	return true;
	//}
	else if (item != NULL && OwningCharacter->Equipment->FindInventoryForItem(item, inventory) && inventory != NULL)
	{
		CurrentInventory = inventory;
		CurrentItemSlot = NULL;
		canStore = true;
	}
	if (canStore)
	{
		CurrentStoreItem = item;
		item->OnStartItemStore.Broadcast(OwningCharacter);
	}
	return canStore;
}

bool UInteractionComponent::TickStoreItem(float deltaTime)
{
	if (CurrentStoreItem == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to store is NULL!"));
		return false;
	}
	if (!CurrentStoreItem->CanBeStored)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be stored!"), *CurrentStoreItem->LocalizedItemName.ToString());
		return false;
	}
	CurrentStoreItem->OnTickItemStore.Broadcast(deltaTime, OwningCharacter);
	return true;
}

bool UInteractionComponent::EndStoreItem(bool abort)
{
	if (CurrentStoreItem == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to store is NULL!"));
		return false;
	}
	if (!CurrentStoreItem->CanBeStored)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be stored!"), *CurrentStoreItem->LocalizedItemName.ToString());
		return false;
	}
	if (!abort)
	{
		CurrentStoreItem->OnEndItemStore.Broadcast(OwningCharacter);
		if (PerformStore(CurrentStoreItem))
		{
			OwningCharacter->Equipment->UnselectItem(true);
		}
	}
	CurrentStoreItem = NULL;
	CurrentInventory = NULL;
	CurrentItemSlot = NULL;
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool UInteractionComponent::CanPickupItem(AItem* item)
{
	if (OwningCharacter->Equipment->HandSelectedItem != NULL)
	{
		UE_LOG(LogCaedeInteraction, Warning,
			TEXT("There is still a hand item: %s - new item %s cannot pickup!"),
			*OwningCharacter->Equipment->HandSelectedItem->InternalItemName,
			*item->InternalItemName);
		return false;
	}

	ABasicItem* basic = Cast<ABasicItem>(item);
	if (basic != NULL)
	{
		//return OwningCharacter->Equipment->CanEquipItem(basic);
		return true;
	}

	AInventoryItem* inventory = Cast<AInventoryItem>(item);
	if (inventory != NULL)
	{
		return true; // OwningCharacter->Equipment->CanEquipInventory(inventory);
	}

	AWeaponItem* weapon = Cast<AWeaponItem>(item);
	if (weapon != NULL)
	{
		return true; // OwningCharacter->Equipment->CanEquipWeapon(weapon);
	}

	AArmorItem* armor = Cast<AArmorItem>(item);
	if (armor != NULL)
	{
		return false; // OwningCharacter->Equipment->CanEquipArmor(armor);
	}


	if (item != NULL)
	{
		return true;
	}

	return false;
}

bool UInteractionComponent::StartPickupItem(AItem* item)
{
	AItem* currentInteraction;
	TEnumAsByte<EItemInteractionType::Type> interactionType;
	if (IsInteractingWithItem(currentInteraction, interactionType))
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("There is currently an item %s in interaction %d!"), *currentInteraction->InternalItemName, (uint8)interactionType);
		return false;
	}

	if (item == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to pickup is NULL!"));
		return false;
	}
	if (!item->CanBePickedUp)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot pickup!"), *item->LocalizedItemName.ToString());
		return false;
	}
	if (OwningCharacter->Equipment->HandSelectedItem != NULL)
	{
		UE_LOG(LogCaedeInteraction, Warning,
			TEXT("There is still a hand item: %s - new item %s cannot pickup!"),
			*OwningCharacter->Equipment->HandSelectedItem->InternalItemName,
			*item->InternalItemName);
		return false;
	}
	CurrentPickupItem = item;
	CurrentItemSlot = item->CurrentItemSlot;
	CurrentInventory = item->CurrentInventory;
	item->OnStartItemGrab.Broadcast(OwningCharacter);
	return true;
}

bool UInteractionComponent::TickPickupItem(float deltaTime)
{
	if (CurrentPickupItem == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to pickup is NULL!"));
		return false;
	}
	if (!CurrentPickupItem->CanBePickedUp)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot pickup!"), *CurrentPickupItem->LocalizedItemName.ToString());
		return false;
	}
	CurrentPickupItem->OnTickItemGrab.Broadcast(deltaTime, OwningCharacter);
	return true;
}

bool UInteractionComponent::EndPickupItem(bool abort)
{
	if (CurrentPickupItem == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to pickup is NULL!"));
		return false;
	}
	if (!CurrentPickupItem->CanBePickedUp)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot pickup!"), *CurrentPickupItem->LocalizedItemName.ToString());
		return false;
	}
	if (!abort)
	{
		//OwningCharacter->Equipment->SelectSilent(item, false);
		CurrentPickupItem->OnEndItemGrab.Broadcast(OwningCharacter);
		PerformPickup(CurrentPickupItem);
	}
	CurrentPickupItem = NULL;
	CurrentItemSlot = NULL;
	CurrentInventory = NULL;
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool UInteractionComponent::StartEquipItem(class AItem* item)
{
	AItem* currentInteraction;
	TEnumAsByte<EItemInteractionType::Type> interactionType;
	if (IsInteractingWithItem(currentInteraction, interactionType))
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("There is currently an item %s in interaction %d!"), *currentInteraction->InternalItemName, (uint8)interactionType);
		return false;
	}

	if (item == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to equip is NULL!"));
		return false;
	}
	if (!item->CanBeEquipped)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be equipped!"), *item->LocalizedItemName.ToString());
		return false;
	}
	CurrentEquipItem = item;
	CurrentItemSlot = item->CurrentItemSlot;
	CurrentInventory = item->CurrentInventory;
	item->OnStartItemEquip.Broadcast(OwningCharacter);
	return true;
}

bool UInteractionComponent::TickEquipItem(float deltaTime)
{
	if (CurrentEquipItem == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to equip is NULL!"));
		return false;
	}
	if (!CurrentEquipItem->CanBeEquipped)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be equipped!"), *CurrentEquipItem->LocalizedItemName.ToString());
		return false;
	}
	CurrentEquipItem->OnTickItemEquip.Broadcast(deltaTime, OwningCharacter);
	return true;

}

bool UInteractionComponent::EndEquipItem(bool abort)
{
	if (CurrentEquipItem == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to equip is NULL!"));
		return false;
	}
	if (!CurrentEquipItem->CanBeEquipped)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be equipped!"), *CurrentEquipItem->LocalizedItemName.ToString());
		return false;
	}
	if (!abort)
	{
		CurrentEquipItem->OnEndItemEquip.Broadcast(OwningCharacter);
		if (PerformEquip(CurrentEquipItem))
		{
			OwningCharacter->Equipment->UnselectItem(true);
		}
	}
	CurrentEquipItem = NULL;
	CurrentItemSlot = NULL;
	CurrentInventory = NULL;
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool UInteractionComponent::StartDropItem(class AItem* item)
{
	AItem* currentInteraction;
	TEnumAsByte<EItemInteractionType::Type> interactionType;
	if (IsInteractingWithItem(currentInteraction, interactionType))
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("There is currently an item %s in interaction %d!"), *currentInteraction->InternalItemName, (uint8)interactionType);
		return false;
	}

	if (item == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to drop is NULL!"));
		return false;
	}
	if (!item->CanBeDropped)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be dropped!"), *item->LocalizedItemName.ToString());
		return false;
	}

	CurrentDropItem = item;
	CurrentItemSlot = item->CurrentItemSlot;
	CurrentInventory = item->CurrentInventory;
	item->OnStartItemDrop.Broadcast(OwningCharacter);
	return true;
}

bool UInteractionComponent::TickDropItem(float deltaTime)
{
	if (CurrentDropItem == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to drop is NULL!"));
		return false;
	}
	if (!CurrentDropItem->CanBeDropped)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be dropped!"), *CurrentDropItem->LocalizedItemName.ToString());
		return false;
	}
	CurrentDropItem->OnTickItemDrop.Broadcast(deltaTime, OwningCharacter);
	return true;
}

bool UInteractionComponent::EndDropItem(bool abort)
{
	if (CurrentDropItem == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("Item to drop is NULL!"));
		return false;
	}
	if (!CurrentDropItem->CanBeDropped)
	{
		UE_LOG(LogCaedeInteraction, Log, TEXT("The item %s cannot be dropped!"), *CurrentDropItem->LocalizedItemName.ToString());
		return false;
	}
	if (!abort)
	{
		CurrentDropItem->OnEndItemDrop.Broadcast(OwningCharacter);
		
		if (PerformDrop(CurrentDropItem))
		{
		}
	}
	CurrentDropItem = NULL;
	CurrentItemSlot = NULL;
	CurrentInventory = NULL;
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool UInteractionComponent::IsInteractingWithItem(AItem*& interactionItem, TEnumAsByte<EItemInteractionType::Type>& interactionType)
{
	if (CurrentDropItem != NULL)
	{
		interactionItem = CurrentDropItem;
		interactionType = EItemInteractionType::Drop;
		return true;
	}
	else if (CurrentEquipItem != NULL)
	{
		interactionItem = CurrentEquipItem;
		interactionType = EItemInteractionType::Equip;
		return true;
	}
	else if (CurrentPickupItem != NULL)
	{
		interactionItem = CurrentPickupItem;
		interactionType = EItemInteractionType::Pickup;
		return true;

	}
	else if (CurrentUseItem != NULL)
	{
		interactionItem = CurrentUseItem;
		interactionType = EItemInteractionType::Use;
		return true;

	}
	else if (CurrentStoreItem != NULL)
	{
		interactionItem = CurrentStoreItem;
		interactionType = EItemInteractionType::Store;
		return true;
	}
	interactionItem = NULL;
	interactionType = EItemInteractionType::Invalid;
	return false;
}


//////////////////////////////////////////////////////////////////////////

bool UInteractionComponent::GetCurrentHoveredActor(AActor*& hoveredActor)
{
	if (CurrentHoveredActor == NULL)
	{
		hoveredActor = NULL;
		return false;
	}
	hoveredActor = CurrentHoveredActor;
	return true;
}

//////////////////////////////////////////////////////////////////////////

// Internal Private (not exposed to BP)

bool UInteractionComponent::PerformPickup(AItem* item)
{
	if (item != NULL)
	{
		return (OwningCharacter->Equipment->SelectItem(item) != NULL);
	}
	return false;
}

bool UInteractionComponent::PerformStore(class AItem* item)
{
	UEquipmentComponent* equipment = OwningCharacter->Equipment;
	UInventoryComponent* inventory = NULL;

	if (item != NULL && equipment->FindInventoryForItem(item, inventory) && inventory != NULL)
	{
		return inventory->StoreItem(item);
	}
	return false;
}

bool UInteractionComponent::PerformUse(AItem* item)
{
	// usage is defined in the item!
	return true;
}

bool UInteractionComponent::PerformEquip(AItem* item)
{
	AWeaponItem* weapon = Cast<AWeaponItem>(item);
	AArmorItem* armor = Cast<AArmorItem>(item);
	AInventoryItem* inventory = Cast<AInventoryItem>(item);

	UEquipmentComponent* equipment = OwningCharacter->Equipment;

	if (item != NULL)
	{
		// TODO: equip item into item-slot

	}
	if (inventory != NULL)
	{
		if (equipment->CanEquipInventory(inventory) && equipment->EquipInventory(inventory))
		{
			return true;
		}
	}
	if (weapon != NULL)
	{
		if (equipment->CanEquipWeapon(weapon) && equipment->EquipWeapon(weapon) > -1)
		{
			return true;
		}
	}
	if (armor != NULL)
	{
		if (equipment->CanEquipArmor(armor) && equipment->EquipArmor(armor))
		{
			return true;
		}
	}
	return false;
}

bool UInteractionComponent::PerformDrop(AItem* item)
{
	AWeaponItem* weaponItem = Cast<AWeaponItem>(item);
	AArmorItem* armorItem = Cast<AArmorItem>(item);
	AInventoryItem* inventoryItem = Cast<AInventoryItem>(item);

	UInventoryComponent* inventory;
	UEquipmentComponent* equipment = OwningCharacter->Equipment;

	bool droppedItem = false;

	if (OwningCharacter->Equipment->HandSelectedItem == item)
	{
		OwningCharacter->Equipment->UnselectItem(true);
		item->SetItemFree(true, true, true);
		droppedItem = true;
	}
	else if (inventoryItem != NULL)
	{
		if (equipment->UnequipInventory(inventoryItem))
		{
			item->SetItemFree(true, true, true);
			droppedItem = true;
		}
	}
	else if (weaponItem != NULL)
	{
		if (equipment->UnequipWeapon(weaponItem))
		{
			item->SetItemFree(true, true, true);
			droppedItem = true;
		}
	}
	else if (armorItem != NULL)
	{
		if (equipment->UnequipArmor(armorItem))
		{
			item->SetItemFree(true, true, true);
			droppedItem = true;
		}
	}
	// check if the item is in an inventory item
	else if (equipment->IsInInventory(item, inventory))
	{
		droppedItem = inventory->DropItem(item);
		item->SetItemFree(true, true, true);
	}

	if (droppedItem)
	{
		item->SetActorHiddenInGame(false);
		item->TogglePhyiscsAndCollision(true);
	}
	return droppedItem;
}

//////////////////////////////////////////////////////////////////////////

void UInteractionComponent::FireBeginHoverEvent(AActor* newHoverActor)
{
	if (newHoverActor == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("The actor to fire the begin-hover-event for is invalid!"));
		return;
	}
	// after end hover the CurrentHoveredActor is NULL!
	// and if the newHoverActor is the CurrentHoveredActor, we do no want to trigger again!
	if (CurrentHoveredActor == NULL)
	{
		OnBeginHoverActor.Broadcast(newHoverActor);
		CurrentHoveredActor = newHoverActor;
	}
	else if (newHoverActor != CurrentHoveredActor)
	{
		FireEndHoverEvent(CurrentHoveredActor);
		OnBeginHoverActor.Broadcast(newHoverActor);
		CurrentHoveredActor = newHoverActor;
	}
}

void UInteractionComponent::FireEndHoverEvent(AActor* oldHoverActor)
{
	if (oldHoverActor == NULL)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("The actor to fire the end-hover-event for is invalid!"));
		return;
	}
	if (oldHoverActor != CurrentHoveredActor)
	{
		UE_LOG(LogCaedeInteraction, Error, TEXT("The actor to fire the end-hover-event for is not the current hovered actor!?!"));
		return;
	}
	OnEndHoverActor.Broadcast(CurrentHoveredActor);
	CurrentHoveredActor = NULL;
}

