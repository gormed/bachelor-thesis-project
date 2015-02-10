// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"
#include "SlotComponent.h"
#include "Slots/ItemSlot.h"
#include "../Items/Item.h"


USlotComponent::USlotComponent(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	bWantsInitializeComponent = true;

	FreeShortcuts.Empty(AvailableShortcuts.Num());
	SlotMap.Empty(InitialSlotDefinitions.Num());
	SlotItemShortcuts.Empty(AvailableShortcuts.Num());

}

TArray<AItemSlot*> USlotComponent::GetSlots(UClass* baseType)
{
	TArray<AItemSlot*> itemArray;
	if (bDisableSlotShortcuts)
	{
		return itemArray;
	}
	if (SlotMap.Contains(baseType))
	{
		itemArray = SlotMap[baseType].SlotIntstances;
	}
	return itemArray;
}

void USlotComponent::InitializeComponent()
{
	// add free slot shortcuts
	for (int32 freeIndex : AvailableShortcuts)
	{
		FreeShortcuts.Add(freeIndex);
	}
	SlotItemShortcuts.Init(FreeShortcuts.Num());

	for (FSlotDefinition def : InitialSlotDefinitions)
	{
		if (def.IsValidDefinition(def.SlotSpecificType))
		{
			CreateSlotFromDefinition(def.SlotBaseType, def.SlotSpecificType, def.SocketNames, GetOwner(), def.SlotItemType);
		}
	}
	UActorComponent::InitializeComponent();
}

void USlotComponent::UninitializeComponent()
{
	for (auto defEntry : SlotMap)
	{
		for (AItemSlot* itemSlot : defEntry.Value.SlotIntstances)
		{
			AItem* item = itemSlot->SlotItem;
			itemSlot->Destroy();
			if (item != NULL)
			{
				item->Destroy();
			}
		}
		defEntry.Value.SlotIntstances.Empty();
	}
	UActorComponent::UninitializeComponent();
}

AItemSlot* USlotComponent::CreateSlotFromDefinition(UClass* slotBaseType, UClass* slotSpecificType, TArray<FName> socketNames, AActor* parentActor, UClass* socketItemClass)
{
	if (slotBaseType == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The slot definition base type is invalid!"));
		return NULL;
	}
	if (slotSpecificType == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The slot definition specific type is invalid!"));
		return NULL;
	}
	AItemSlot* slot = NULL;
	FSlotDefinition def;
	if (SlotMap.Contains(slotBaseType))
	{
		FSlotDefinition& definition = SlotMap[slotBaseType];
		if (definition.IsValidDefinition(slotSpecificType))
		{
			/*FName name = slotSpecificType->GetFName();
			FString appendix("_");
			appendix.AppendInt(definition.SlotIntstances.Num() + 1);
			name.AppendString(appendix);*/
			slot = World->SpawnActor<AItemSlot>(slotSpecificType);
		}
		def = definition;
	}
	else
	{
		FSlotDefinition definition;
		definition.SlotBaseType = slotBaseType;
		if (definition.IsValidDefinition(slotSpecificType))
		{
			slot = World->SpawnActor<AItemSlot>(slotSpecificType);
		}
		SlotMap.Add(slotBaseType, definition);
		SlotBaseTypes.Add(slotBaseType);
		def = definition;
	}

	if (slot != NULL && SlotMap.Contains(slotBaseType))
	{
		HandleSlotSpecifics(slot, slotBaseType);
		ACaedeCharacter* character = NULL;
		AItem* item = NULL;
		slot->OwnSlot(this);
		if ((character = Cast<ACaedeCharacter>(parentActor)) != NULL)
		{
			slot->InitializeSockets(character->GetMesh(), socketNames);
			
		}
		else if ((item = Cast<AItem>(parentActor)) != NULL)
		{
			slot->InitializeSockets(item->ItemMesh, socketNames);
		}
		if (socketItemClass != NULL)
		{
			AItem* socketItem = World->SpawnActor<AItem>(socketItemClass);
			if (socketItem != NULL && !slot->IsOccupied() && slot->Fits(socketItem) && slot->Insert(socketItem))
			{
				UE_LOG(LogCaedeSlot, Log, TEXT("Spawned and inserted item %s!"), *socketItem->GetName());
			}
			else
			{
				UE_LOG(LogCaedeSlot, Warning, TEXT("Failed to spawn class %s and add initial item to socket %s!"), *socketItemClass->GetName(), *slot->GetName());
			}
		}
		SlotMap[slotBaseType].SlotIntstances.Add(slot);
		OnSlotCreated.Broadcast(slot, slotBaseType);
		UE_LOG(LogCaedeSlot, Log, TEXT("The slot %s was constucted and added successful!"), *slot->GetName());
	}
	else
	{
		UE_LOG(LogCaedeSlot, Warning, TEXT("The slot for %s could not be constucted!"), *slotSpecificType->GetName());
	}

	return slot;
}

void USlotComponent::HandleSlotSpecifics(AItemSlot* slot, UClass* baseType)
{
	if (!SlotMap.Contains(baseType))
	{
		UE_LOG(LogCaedeSlot, Warning, TEXT("The specific base type %s was not found as a valid slot!"), *baseType->GetName());
		return;
	}
	if (slot == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The slot to handle is invalid!"));
		return;
	}

	int32 freeSlotIndex = -1;

	if (FreeShortcuts.Num() > 0)
	{
		for (int32 slotIdx : FreeShortcuts)
		{
			freeSlotIndex = slotIdx;
			break;
		}
		//FreeShortcuts.Remove(freeSlotIndex);
	}

	if (!BindSlotToShortcut(slot, freeSlotIndex, false))
	{
		FreeShortcuts.Add(freeSlotIndex);
	}
}

bool USlotComponent::RemoveSlot(AItemSlot* remove, UClass* baseType)
{
	if (remove == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The slot to remove is invalid!"));
		return false;
	}
	if (baseType == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The base type to check is invalid!"));
		return false;
	}
	if (SlotItemShortcuts.Contains(remove) && SlotItemShortcuts.IsValidIndex(remove->SlotIndex))
	{
		UnbindSlotFromShortcut(remove->SlotIndex);
	}
	if (SlotMap.Contains(baseType) && SlotMap[baseType].SlotIntstances.Contains(remove))
	{
		// remove item if present
		if (remove->IsOccupied())
		{
			remove->Remove();
		}
		// remove from components slot map 
		int32 removeCount = SlotMap[baseType].SlotIntstances.Remove(remove);
		OnSlotRemoved.Broadcast(remove, baseType);
		UE_LOG(LogCaedeSlot, Log, TEXT("Deleted slot %s from %s's slot component!"), *remove->GetName(), *GetOwner()->GetName());
		return removeCount > 0;
	}
	UE_LOG(LogCaedeSlot, Warning, TEXT("The slot to remove is not in this component!"));
	return false;
}

bool USlotComponent::ContainsSlot(AItemSlot* slot, UClass* baseType)
{
	if (slot == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The slot to check is invalid!"));
		return false;
	}
	if (baseType == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The base type to check is invalid!"));
		return false;
	}
	return SlotMap.Contains(baseType) 
		&& SlotMap[baseType].SlotIntstances.Contains(slot);
}

bool USlotComponent::ContainsItem(AItem* item, UClass* baseType, AItemSlot*& outSlot)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The item to find a slot for is invalid!"));
		outSlot = NULL;
		return false;
	}
	if (baseType == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The base type to check is invalid!"));
		outSlot = NULL;
		return false;
	}
	return SlotMap.Contains(baseType) && SlotMap[baseType].ContainsItem(item, outSlot);
}

bool USlotComponent::FindSlotForItem(AItem* item, UClass* baseType, AItemSlot*& foundSlot)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The item to find a slot for is invalid!"));
		foundSlot = NULL;
		return false;
	}
	if (baseType == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The base type to check is invalid!"));
		foundSlot = NULL;
		return false;
	}
	if (SlotMap.Contains(baseType))
	{
		for (AItemSlot* slot : SlotMap[baseType].SlotIntstances)
		{
			if (!slot->IsOccupied() && slot->Fits(item))
			{
				UE_LOG(LogCaedeSlot, Log, TEXT("Found slot %s for item %s!"), *slot->GetName(), *item->InternalItemName);
				foundSlot = slot;
				return true;
			}
		}
	}
	UE_LOG(LogCaedeSlot, Warning, TEXT("Found no slot for item %s!"), *item->InternalItemName);
	foundSlot = NULL;
	return false;
}

//////////////////////////////////////////////////////////////////////////

bool USlotComponent::GetSlotFromShortcut(int32 shortcut, AItemSlot*& slot)
{
	if (bDisableSlotShortcuts)
	{
		UE_LOG(LogCaedeSlot, Log, TEXT("Shortcuts are disabled!"));
		return false;
	}
	if (SlotItemShortcuts.IsValidIndex(shortcut) && SlotShortcuts.Contains(shortcut))
	{
		if (SlotShortcuts[shortcut] != NULL && SlotItemShortcuts[shortcut] != NULL)
		{
			slot = SlotShortcuts[shortcut];
			UE_LOG(LogCaedeSlot, Log, TEXT("Found and returned slot %s at shortcut %d!"), *slot->GetName(), shortcut);
			return true;
		}
	}
	UE_LOG(LogCaedeSlot, Warning, TEXT("Unable to get shortcut %d!"), shortcut);
	return false;
}

bool USlotComponent::BindSlotToShortcut(AItemSlot* itemSlot, int32 index, bool forceUnbindOld)
{
	if (bDisableSlotShortcuts)
	{
		UE_LOG(LogCaedeSlot, Log, TEXT("Shortcuts are disabled!"));
		return false;
	}
	AArmorSlot* armorSlot = Cast<AArmorSlot>(itemSlot);

	if (armorSlot != NULL)
	{
		// no armor shortcuts!
		UE_LOG(LogCaedeSlot, Error, TEXT("Cannot bind armor slot %s to a shortcut!"), *armorSlot->GetName());
		return false;
	}

	if (SlotShortcuts.Contains(index))
	{
		if (forceUnbindOld)
		{
			UnbindSlotFromShortcut(index);
		}
		else
		{
			UE_LOG(LogCaedeSlot, Warning, TEXT("Shortcut already bound to another slot, nothing happened!"));
			return false;
		}
	}

	if (FreeShortcuts.Contains(index) && SlotItemShortcuts.IsValidIndex(index))
	{
		FreeShortcuts.Remove(index);
		itemSlot->SlotIndex = index;
		SlotShortcuts.Add(itemSlot->SlotIndex, itemSlot);
		SlotItemShortcuts[index] = itemSlot;

		UE_LOG(LogCaedeSlot, Log, TEXT("Bound slot %s to shortcut %d"), *itemSlot->GetName(), index);
		return true;
	}
	else
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The index (%d) that is meant to be used is not available!"), index);
		return false;
	}
}

AItemSlot* USlotComponent::UnbindSlotFromShortcut(int32 index)
{
	if (bDisableSlotShortcuts)
	{
		UE_LOG(LogCaedeSlot, Log, TEXT("Shortcuts are disabled!"));
		return false;
	}
	if (SlotShortcuts.Contains(index) && SlotItemShortcuts.IsValidIndex(index))
	{
		AItemSlot* itemSlot = SlotShortcuts[index];
		SlotItemShortcuts[index] = NULL;
		SlotShortcuts.Remove(index);
		FreeShortcuts.Add(itemSlot->SlotIndex);
		itemSlot->SlotIndex = -1;
		UE_LOG(LogCaedeSlot, Log, TEXT("Unbound slot %s from shortcut %d"), *itemSlot->GetName(), index);
		return itemSlot;
	}
	else
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("No slot found for shortcut %d, nothing happened!"), index);
		return NULL;
	}
}

//////////////////////////////////////////////////////////////////////////

TArray<AItem*> USlotComponent::GetAllItemsOfBaseType(UClass* baseType, UClass* baseItemType)
{
	TArray<AItem*> itemsOfBaseType;
	if (baseType == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The base type to check is invalid!"));
		return itemsOfBaseType;
	}

	if (SlotMap.Contains(baseType))
	{
		for (AItemSlot* itemSlot : SlotMap[baseType].SlotIntstances)
		{
			if (itemSlot->IsOccupied() && itemSlot->SlotItem->GetClass()->IsChildOf(baseType))
			{
				itemsOfBaseType.Add(itemSlot->SlotItem);
			}
		}

	}
	return itemsOfBaseType;
}

AItem* USlotComponent::GetSpecificItem(UClass* baseType, UClass* baseItemType, UClass* specificItemType)
{
	if (baseType == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The base type to check is invalid!"));
		return NULL;
	}
	TArray<AItem*> baseTypeItems = GetAllItemsOfBaseType(baseType, baseItemType);
	for (AItem* item : baseTypeItems)
	{
		if (item != NULL && item->GetClass()->IsChildOf(specificItemType))
		{
			return item;
		}
	}
	return NULL;
}
