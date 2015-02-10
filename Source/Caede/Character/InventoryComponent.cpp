

#include "Caede.h"
#include "Character/InventoryComponent.h"
#include "Character/CaedeCharacter.h"
#include "Character/EquipmentComponent.h"
#include "Items/Item.h"
#include "Items/WeaponItem.h"


UInventoryComponent::UInventoryComponent(const class FObjectInitializer& PCIP)
: Super(PCIP)
{
	HasWeight = true;
	HasCount = false;
	InventoryItemClass = AItem::StaticClass();

	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostPhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	bWantsInitializeComponent = true;

}

bool UInventoryComponent::AddItem(AItem* item)
{
	// return false for a invalid item class!
	if (item == NULL)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Invalid item, make sure it is not NULL for this inventory (%s)!"), *GetName());
		return false;
	}
	if (!item->GetClass()->IsChildOf(InventoryItemClass))
	{
		UE_LOG(LogCaedeInventory, Warning, TEXT("Invalid item class, the item (of class: %s) doesn't have the class of InventoryItemClass (is %s) for this inventory (%s)!"), *item->GetClass()->GetName(), *InventoryItemClass->GetName(), *GetName());
		return false;
	}
	if (ContainsItem(item))
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Item %s is already in the inventory %s!"), *item->LocalizedItemName.ToString(), *GetName());
		return false;
	}
	// return false if inventory is full
	if (HasWeight && CurrentInventoryWeight + item->ActualWeight() > MaxInventoryWeight)
	{
		UE_LOG(LogCaedeInventory, Log, TEXT("Inventory %s is full, cannot add item %s (weight: %d)! Current inventory weight is %d"), *GetName(), *item->LocalizedItemName.ToString(), item->ActualWeight(), CurrentInventoryWeight);
		return false;
	}
	// return false if inventory is full
	if (HasCount && CurrentInventoryCount + 1 > MaxInventoryCount)
	{
		UE_LOG(LogCaedeInventory, Log, TEXT("Inventory %s is full, cannot add item %s! Current inventory count is %d"), *GetName(), *item->LocalizedItemName.ToString(),CurrentInventoryCount);
		return false;
	}
	// else we can add the item, change owner, and add the weight
	//InventoryArray.Add(item);
	DeepAdd(item->GetClass(), item);

	// Update the weight and count
	CurrentInventoryWeight += item->ActualWeight();
	CurrentInventoryCount++;
	// own the item
	item->SetItemInventory(this);
	// in inventory, the item is invisible
	item->ItemMesh->SetVisibility(false, true);
	OnItemAdded.Broadcast(item->GetClass(), InventoryMap[item->GetClass()], item);
	OnInventoryChanged.Broadcast(this);

	return true;
}

bool UInventoryComponent::StoreItem(AItem* item)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Item to be picked up is NULL!"));
		return false;
	}
	if (item->CurrentInventory == this)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Item to be picked up is already in the invenotry, hups!"));
		return false;
	}
	if (!item->CanBePickedUp)
	{
		UE_LOG(LogCaedeInventory, Log, TEXT("Item %s cannot be picked up!"), *item->LocalizedItemName.ToString());
		return false;
	}
	if (item->CurrentInventory != NULL)
	{
		item->OnItemChangedOwner.Broadcast(item->CurrentInventory, this);
		UE_LOG(LogCaedeInventory, Log, TEXT("Item %s has a owner and cannot be picked up!"), *item->LocalizedItemName.ToString());
	}
	if (AddItem(item))
	{
		item->OnItemPickedUp.Broadcast(this);
		return true;
	}
	return false;
}

bool UInventoryComponent::GetItems(TSubclassOf<AItem> itemClass, FItemClassCollection& itemCollection)
{
	if (itemClass == NULL)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Item class is NULL!"));
		return false;
	}
	if (InventoryMap.Contains(itemClass))
	{
		itemCollection = InventoryMap[itemClass];
		return true;
	}
	else
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Error while getting items of class '%s' from inventory '%s', item class not present!"), *itemClass->GetName(), *this->InventoryName.ToString());
		return false;
	}
}

bool UInventoryComponent::RemoveItem(AItem* item)
{
	if (DeepRemove(item->GetClass(), item))
	{
		CurrentInventoryWeight -= (item->ActualWeight());
		CurrentInventoryCount--;

		item->ItemMesh->SetVisibility(true, true);
		UClass* itemClass = item->GetClass();
		OnItemRemoved.Broadcast(itemClass,
			InventoryMap.Contains(itemClass) ?
				InventoryMap[itemClass] :
				FItemClassCollection(), 
			item);
		OnInventoryChanged.Broadcast(this);

		UE_LOG(LogCaedeInventory, Log, TEXT("Dropped item %s times!"), *item->InternalItemName);
		return true;
	}
	UE_LOG(LogCaedeInventory, Error, TEXT("Item %s is not in this inventory!"), *item->InternalItemName);
	return false;
}

bool UInventoryComponent::DropItem(AItem* item)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Item to be dropped is NULL!"));
		return false;
	}
	
	if(!item->CanBeDropped)
	{
		UE_LOG(LogCaedeInventory, Log, TEXT("Item %s cannot be dropped!"), *item->InternalItemName);
		return false;
	}
	if (RemoveItem(item))
	{
		item->OnItemDropped.Broadcast(this);
		item->SetItemFree(true, true, false);
		return true;
	}
	return false;
}

bool UInventoryComponent::GiveItem(AItem* item, ACaedeCharacter* character)
{
	if (character == NULL)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("The character to give a item to is NULL!"));
		return false;
	}
	UInventoryComponent* destinatedInventory = NULL;
	if (!character->Equipment->FindInventoryForItem(item, destinatedInventory))
	{
		UE_LOG(LogCaedeInventory, Log, TEXT("The item %s cannot be given to character %s because there is no suitable inventory for it!"), *item->InternalItemName, *character->GetName());
		return false;
	}

	return DeepMove(item, destinatedInventory);
}

bool UInventoryComponent::MoveItem(AItem* item, UInventoryComponent* inventory)
{
	return DeepMove(item, inventory);
}

bool UInventoryComponent::MoveItemCollection(TSubclassOf<AItem> itemClass, UInventoryComponent* inventory)
{
	FItemClassCollection classCollectionToMove;
	if (GetItems(itemClass, classCollectionToMove))
	{
		for (AItem* collectionItem : classCollectionToMove.ItemCollection)
		{
			MoveItem(collectionItem, inventory);
		}
		return true;
	}
	return false;
}

bool UInventoryComponent::ContainsItem(AItem* item)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Item to be checked is NULL!"));
		return false;
	}
	if (InventoryMap.Contains(item->GetClass()))
	{
		return InventoryMap[item->GetClass()].ItemCollection.Contains(item);
	}
	return false;
}

bool UInventoryComponent::FitsItem(AItem* item)
{
	if (ContainsItem(item))
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Item %s already in inventory!"), *item->LocalizedItemName.ToString());
		return false;
	}

	// return false for a invalid item class!
	if (!item->GetClass()->IsChildOf(InventoryItemClass))
	{
		UE_LOG(LogCaedeInventory, Log, TEXT("Invalid item class (is %s for inventory and %s for item) for this inventory (%s) - Item does not fit!"), *InventoryItemClass->GetName(), *item->GetClass()->GetName(), *GetName());
		return false;
	}
	// return false if inventory is full
	if (HasWeight && CurrentInventoryWeight + item->ActualWeight() > MaxInventoryWeight)
	{
		UE_LOG(LogCaedeInventory, Log, TEXT("Inventory %s is full, cannot add item %s (weight: %d)! Current inventory weight is %d - Item does not fit!"), *GetName(), *item->LocalizedItemName.ToString(), item->ActualWeight(), CurrentInventoryWeight);
		return false;
	}
	// return false if inventory is full
	if (HasCount && CurrentInventoryCount + 1 > MaxInventoryCount)
	{
		UE_LOG(LogCaedeInventory, Log, TEXT("Inventory %s is full, cannot add item %s! Current inventory count is %d - Item does not fit!"), *GetName(), *item->LocalizedItemName.ToString(), CurrentInventoryCount);
		return false;
	}
	return true;
}

bool UInventoryComponent::IsEmpty()
{
	if (HasWeight)
	{
		return CurrentInventoryWeight <= 0;
	}
	else if (HasCount)
	{
		return CurrentInventoryCount <= 0;
	}
	else
	{
		return InventoryMap.Num() <= 0;
	}
}

TArray<FItemClassCollection> UInventoryComponent::GetItemsSortedByType()
{
	TArray<FItemClassCollection> finalSorted;

	for (auto itemArray : InventoryMap)
	{
		finalSorted.Add(itemArray.Value);
	}

	return finalSorted;
}

//////////////////////////////////////////////////////////////////////////

// Internal (not exposed in BP)

bool UInventoryComponent::DeepAdd(UClass* itemClass, AItem* item)
{
	if (itemClass == NULL)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Item class is NULL!"));
		return false;
	}
	if (item == NULL)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Item is NULL!"));
		return false;
	}
	// Check if class is already present
	if (InventoryMap.Contains(itemClass))
	{
		InventoryMap[itemClass].ItemCollection.Add(item);
		return true;
	}
	// else we have to create it
	else
	{
		// init item collection with class, prototype item and the fresh array
		FItemClassCollection newItemCollection;
		newItemCollection.ItemClass = itemClass;
		newItemCollection.Prototype = item;
		newItemCollection.ItemCollection.Add(item);
		// add the item collection to the inventory map
		InventoryMap.Add(newItemCollection.ItemClass, newItemCollection);
		// also add the item to the general array with all prototype items
		// use the prototype pointer to ensure it always points to the actual prototype item
		InventoryArray.Add(newItemCollection.Prototype);
		return true;
	}
}

bool UInventoryComponent::DeepRemove(UClass* itemClass, AItem* item)
{
	if (itemClass == NULL)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Item class is NULL!"));
		return false;
	}
	if (item == NULL)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Item is NULL!"));
		return false;
	}
	// Check if class is present
	if (InventoryMap.Contains(itemClass))
	{
		// check if we remove the prototype
		if (InventoryMap[itemClass].Prototype == item)
		{
			// if there is sth left in the collection
			if (InventoryMap[itemClass].ItemCollection.Num() > 1)
			{
				// remove the item
				InventoryMap[itemClass].ItemCollection.Remove(item);
				// set a new prototype
				InventoryMap[itemClass].Prototype = InventoryMap[itemClass].ItemCollection.Last();
			}
			else
			{
				InventoryArray.Remove(InventoryMap[itemClass].Prototype);
				InventoryMap.Remove(itemClass);
				
			}
		}
		// otherwise we simply remove the item from the collection array
		else
		{
			InventoryMap[itemClass].ItemCollection.Remove(item);
		}
		return true;
	}
	// else there is nothing to remove and there must have been an error
	else
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Error while removing item '%s' with class '%s' from inventory '%s', item class not present!"), *item->LocalizedItemName.ToString(), *itemClass->GetName(), *this->InventoryName.ToString());
		return false;
	}
}

bool UInventoryComponent::DeepMove(AItem* item, UInventoryComponent* inventory)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Item to be given is NULL!"));
		return false;
	}
	if (inventory == NULL)
	{
		UE_LOG(LogCaedeInventory, Error, TEXT("Target inventory is NULL, cannot give %s away!"), *item->LocalizedItemName.ToString());
		return false;
	}

	if (!item->CanBeDropped)
	{
		UE_LOG(LogCaedeInventory, Log, TEXT("Item %s cannot be given to anybody!"), *item->LocalizedItemName.ToString());
		return false;
	}
	if (inventory->FitsItem(item) && RemoveItem(item))
	{
		if (inventory->StoreItem(item))
		{
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

void UInventoryComponent::InitializeComponent()
{
	OwningItem = Cast<AInventoryItem>(GetOwner());
}
