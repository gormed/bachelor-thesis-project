// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedPointer.h"
#include "ItemSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotItemInsertedSignature, class AItem*, inserted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotItemRemovedSignature, class AItem*, removed);

class AItem;
class ACaedeCharacter;

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API AItemSlot : public AActor
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Slot")
	USlotComponent* OwningComponent;

	UFUNCTION(BlueprintCallable, Category = "Slot", meta = (FriendlyName = "Own Slot By A Character"))
	void OwnSlot(USlotComponent* newOwner);

	UFUNCTION(BlueprintCallable, Category = "Slot", meta = (FriendlyName = "Unown This Slot"))
	USlotComponent* UnownSlot();

	//////////////////////////////////////////////////////////////////////////

	/// Gets called by the 'SlotComponent' on initialize
	bool InitializeSockets(USceneComponent* parentComponent, TArray<FName> socketNames);

	UFUNCTION(BlueprintCallable, Category = "Slot Socket", meta = (FriendlyName = "Get First Available Socket Name"))
	FName GetFirstAvailableSocketName();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Slot Socket")
	TArray<FName> AvailableSocketNames;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Slot Socket")
	FName CurrentSocketName;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Slot Socket")
	USceneComponent* ParentComponent;

	//////////////////////////////////////////////////////////////////////////

	///*
	//	The 3D sockets of this slot, where items will be attached to
	//	Either the skeleton- or the mesh-socket must be used to prevent errors, but one of both can be None/NULL
	//	
	//	This is for skeleton/bone sockets
	//*/
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Slot")
	//class USkeletalMeshSocket* SkeletonSocket;

	///*
	//	The 3D sockets of this slot, where items will be attached to.
	//	Either the skeleton- or the mesh-socket must be used to prevent errors, but one of both can be None/NULL
	//	
	//	This is for mesh sockets
	//*/
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Slot")
	//class UStaticMeshSocket* MeshSocket;

	//////////////////////////////////////////////////////////////////////////

	/// All item classes that fit into this slot
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Slot")
	TArray<class UClass*>	SlotItemClasses;

	/// The index of this slot
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Slot")
	int32					SlotIndex;

	/// The item in this slot
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Slot")
	AItem*					SlotItem;

	//////////////////////////////////////////////////////////////////////////

	///// Keeps physics simulation of the slot item active
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Slot Flags")
	//bool bMaintainPhysics;

	/// Keeps collision of the slot item active
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Slot Flags")
	bool					bMaintainPhysiscsAndCollision;

	/// Hides the item if inserted to slot
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Slot Flags")
	bool					bHideOnInserted;

	/// If the slot gets destroyed, this also will destroy the item, instead of the usual drop
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Slot Flags")
	bool					bSlotKillsItem;

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintAssignable, Category = "Slot Item Events")
	FOnSlotItemInsertedSignature OnSlotItemInserted;

	UPROPERTY(BlueprintAssignable, Category = "Slot Item Events")
	FOnSlotItemRemovedSignature OnSlotItemRemoved;

	//////////////////////////////////////////////////////////////////////////

	/// Implement this to provide specific behavior of what the socket name is, to insert the item
	/// Native implementation will check all 'AvailableSocketNames' and return the first that fits
	UFUNCTION(BlueprintNativeEvent, Category = "Slot Events", meta = (FriendlyName = "On Item Inserted"))
	FName CanItemInsert(AItem* item);

	/// Implement this to provide specific behavior of what happens when a item gets removed
	/// Native implementation will simply return true
	UFUNCTION(BlueprintNativeEvent, Category = "Slot Events", meta = (FriendlyName = "On Item Removed"))
	bool CanItemRemove(AItem* item, FName removeSocketName);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Slot Behavior", meta = (FriendlyName = "Drop Item From Slot"))
	bool DropItemFromSlot();

	UFUNCTION(BlueprintCallable, Category = "Slot Behavior", meta = (FriendlyName = "Insert Item Into Slot"))
	bool InsertItemIntoSlot(AItem* item);

	UFUNCTION(BlueprintCallable, Category = "Slot Behavior", meta = (FriendlyName = "Move Item To Other Slot"))
	bool MoveItemToOtherSlot(AItemSlot* otherSlot);

	//////////////////////////////////////////////////////////////////////////

	/// Inserts a item to this slot, checking for the 'AllowedItemClasses', returns true if inserted
	virtual bool Insert(AItem* item);

	/// Checks if a certain item fits into this slot
	virtual bool Fits(AItem* item) const;

	/// Removes a item from this slot, returns the item if removed otherwise NULL
	virtual AItem* Remove();

	/// Check if the slot is in use
	virtual bool IsOccupied() const;

	/// Drops the item on destroy and removes the item from the slot component
	virtual void ReceiveDestroyed() override;

	/// Destroys the item if 'bSlotKillsItem' is set
	void DestroyItem();

	virtual void ReceiveEndPlay(EEndPlayReason::Type EndPlayReason) override;

	//////////////////////////////////////////////////////////////////////////

	// Linking must be implemented by the specific slot

	/// Checks if this slot is linked to another
	virtual bool IsLinked() const;

	/// links this slot with the given one
	virtual void Link(AItemSlot* slot);

	/// unlinks from the given slot
	virtual void Unlink(AItemSlot* slot);

	//////////////////////////////////////////////////////////////////////////

	inline bool operator==(const AItemSlot* a) const
	{
		if (a->SlotIndex == SlotIndex && a->SlotItem == SlotItem)
		{
			return true;
		}
		return false;
	}

	inline bool operator!=(const AItemSlot* a) const
	{
		if (a->SlotIndex != SlotIndex || a->SlotItem != SlotItem)
		{
			return true;
		}
		return false;
	}


	//////////////////////////////////////////////////////////////////////////

protected:

	/// All slots this weapon is linked to
	TArray<AItemSlot*> LinkedSlots;
};
