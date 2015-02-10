// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"
#include "ItemSlot.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/StaticMeshSocket.h"
#include "Engine/EngineTypes.h"


AItemSlot::AItemSlot(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	SlotIndex = -1;
}

//////////////////////////////////////////////////////////////////////////

void AItemSlot::OwnSlot(USlotComponent* newOwner)
{
	OwningComponent = newOwner;
	SetOwner(OwningComponent->GetOwner());
	SnapRootComponentTo(OwningComponent->GetOwner());
}

USlotComponent* AItemSlot::UnownSlot()
{
	USlotComponent* lastOwner = OwningComponent;
	OwningComponent = NULL;
	DetachRootComponentFromParent(true);
	return lastOwner;
}

//////////////////////////////////////////////////////////////////////////

FName AItemSlot::CanItemInsert_Implementation(AItem* item)
{
	return GetFirstAvailableSocketName();
}

bool AItemSlot::CanItemRemove_Implementation(AItem* item, FName removeSocketName)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool AItemSlot::DropItemFromSlot()
{
	if (IsOccupied())
	{
		AItem* removed = Remove();
		return true;
	}
	UE_LOG(LogCaedeSlot, Warning, TEXT("There was nothing in slot %s to drop?!"), *GetName());
	return false;
}

bool AItemSlot::InsertItemIntoSlot(AItem* item)
{
	if (IsOccupied())
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The slot %s is already occupied!"), *GetName());
		return false;
	}
	if (Fits(item))
	{
		return Insert(item);
	}
	return false;
}

bool AItemSlot::MoveItemToOtherSlot(AItemSlot* otherSlot)
{
	if (otherSlot == NULL)
	{
		UE_LOG(LogCaede, Error, TEXT("The other slot to move item to is NULL!"));
		return false;
	}
	if (!IsOccupied())
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The slot %s is not occupied!"), *GetName());
		return false;
	}

	AItem* item = Remove();
	return InsertItemIntoSlot(item);
}

//////////////////////////////////////////////////////////////////////////

bool AItemSlot::Insert(AItem* item)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The item to insert into the slot is invalid"));
		return false;
	}
	if (ParentComponent == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The slot was not initialized with 'InitializeSockets' correctly, there is no 'ParentComponent'!"));
		return false;
	}
	// check if the item would fit, retrieve the CurrentSocketName and check if its valid/not none
	if (Fits(item) 
		&& (CurrentSocketName = CanItemInsert(item)).IsValid() 
		&& !CurrentSocketName.IsNone())
	{
		// checks if is the item valid and the socket name existent
		if (item != NULL && ParentComponent->CanAttachAsChild(item->ItemMesh, CurrentSocketName))
		{
			SlotItem = item;
			item->SetItemSlot(this);
			OnSlotItemInserted.Broadcast(SlotItem);
			// Check if the item can attach, and do so
			if (SlotItem->AttachItem(this, ParentComponent, CurrentSocketName))
			{
				// check for the slot flags:
				// maintain physics and/or collision or hide on inserted
				SlotItem->TogglePhyiscsAndCollision(bMaintainPhysiscsAndCollision ? true : false);
				SlotItem->SetActorHiddenInGame(bHideOnInserted ? true : false);

				UE_LOG(LogCaedeSlot, Log, TEXT("Inserted item %s into slot %s!"), *item->InternalItemName, *GetName());
				// and we are done - item inserted
				return true;
			}
			// there must be an error...
			UE_LOG(LogCaedeSlot, Error, TEXT("Failed to attach item %s to slot %s on socket name %s!"),
				*item->InternalItemName, *this->GetName(), *CurrentSocketName.ToString());
			return false;
		}
	}		
	UE_LOG(LogCaedeSlot, Error, TEXT("The item to insert into the slot may not fit, or there is no socket for the item!"));
	return false;
}

AItem* AItemSlot::Remove()
{
	if (!IsOccupied())
	{
		UE_LOG(LogCaedeSlot, Warning, TEXT("The slot is empty, there is nothing to remove!"));
		return NULL;
	}
	else if (CanItemRemove(SlotItem, CurrentSocketName))
	{
		// copy old item
		AItem* item = SlotItem;
		// detach item
		item->DetachItem();
		// make it visible
		item->SetActorHiddenInGame(false);
		// enable the collision
		item->TogglePhyiscsAndCollision(true);
		// simulate the phyiscs
		item->ItemMesh->SetSimulatePhysics(true);
		// invalidate the current socket name
		CurrentSocketName = FName();
		// unlink all slots from this
		for (int32 i = 0; i < LinkedSlots.Num(); i++)
		{
			LinkedSlots[i]->Unlink(this);
		}
		LinkedSlots.Empty();
		// invalidate slot item
		SlotItem = NULL;
		OnSlotItemRemoved.Broadcast(item);
		item->SetItemFree(true, true, true);
		UE_LOG(LogCaedeSlot, Log, TEXT("Removed item %s from slot %s!"), *item->InternalItemName, *GetName());
		return item;
	}
	UE_LOG(LogCaedeSlot, Warning, TEXT("'OnItemRemoved' Event returned false, nothing happened!"));
	return NULL;
}

bool AItemSlot::Fits(AItem* item) const
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The item to check for fit the slot is NULL"));
		return false;
	}
	if (SlotItemClasses.Num() == 0)
	{
		UE_LOG(LogCaedeSlot, Warning, TEXT("The 'SlotItemClasses' Array is empty, there won't fit any item!"));
		return false;
	}
	// iterate all allowed item classes
	for (UClass* itemClass : SlotItemClasses)
	{
		// checks if the items class is inheriting one of the item class
		if (item->GetClass()->IsChildOf(itemClass))
		{
			// then we fit into it! yay
			return true;
		}
	}
	// does not fit at all
	UE_LOG(LogCaedeSlot, Log, TEXT("The item (%s) does not fit into the slot!"), *item->InternalItemName);
	return false;
}

bool AItemSlot::IsOccupied() const
{
	return SlotItem != NULL;
}

bool AItemSlot::IsLinked() const
{
	return LinkedSlots.Num() > 0;
}

void AItemSlot::Link(AItemSlot* slot)
{
	if (slot == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The slot to link is NULL"));
		return;
	}

	if (!LinkedSlots.Contains(slot))
	{
		LinkedSlots.Add(slot);
	}
	else
	{
		UE_LOG(LogCaedeSlot, Warning, TEXT("The slot to link is already linked!"));
	}
}

void AItemSlot::Unlink(AItemSlot* slot)
{
	if (slot == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The slot to unlink is NULL"));
		return;
	}
	LinkedSlots.Remove(slot);
}

bool AItemSlot::InitializeSockets(USceneComponent* parentComponent, TArray<FName> socketNames)
{
	if (parentComponent == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The parent component to attach to is invalid!"));
		return false;
	}
	// inits the socket names (from a FSlotDefinition)
	AvailableSocketNames.Append(socketNames);
	// the parent item mesh or character mesh (important is: USkeletalMeshComponent or UStaticMeshComponent only!)
	ParentComponent = parentComponent;
	UE_LOG(LogCaedeSlot, Log, TEXT("The slot was initialized with %d sockets and parent component %s!"), socketNames.Num(), *parentComponent->GetName());
	return false;
}

FName AItemSlot::GetFirstAvailableSocketName()
{
	if (ParentComponent == NULL)
	{
		UE_LOG(LogCaedeSlot, Error, TEXT("The slot was not initialized with 'InitializeSockets' correctly, there is no 'ParentComponent'!"));
		return FName();
	}
	// checks all AvailableSocketNames for the first survivor
	for (FName socketName : AvailableSocketNames)
	{
		if (!socketName.IsValid())
		{
			UE_LOG(LogCaedeSlot, Error, TEXT("The socket name to insert into invalid!"));
			continue;
		}
		else if (socketName.IsNone())
		{
			UE_LOG(LogCaedeSlot, Error, TEXT("The socket name to insert into the slot is 'None'!"));
			continue;
		}
		else if (!ParentComponent->DoesSocketExist(socketName))
		{
			UE_LOG(LogCaedeSlot, Error, TEXT("The socket name to insert into does not exist in %s!"), *ParentComponent->GetName());
			continue;
		}
		else
		{
			// all traps survived, this is the first valid socket name
			return socketName;
		}
	}
	// None if we didn't find one
	UE_LOG(LogCaedeSlot, Error, TEXT("No socket name found on component %s in %d available socket names!"),
		*ParentComponent->GetName(), AvailableSocketNames.Num());
	return FName();
}

void AItemSlot::ReceiveDestroyed()
{
	if (OwningComponent != NULL)
	{
		// remove from owners SlotMap on removal
		OwningComponent->RemoveSlot(this, AItemSlot::StaticClass());
	}
	DestroyItem();

	AActor::ReceiveDestroyed();
}

void AItemSlot::ReceiveEndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::ReceiveEndPlay(EndPlayReason);
}

void AItemSlot::DestroyItem()
{
	if (bSlotKillsItem && SlotItem != NULL)
	{
		// if we should destroy the item, do so!
		SlotItem->Destroy();
	}
}
