

#pragma once

#include "Components/ActorComponent.h"

#include "InventoryComponent.generated.h"

class AItem;
class ACaedeCharacter;
class AWeaponItem;
class UInventoryComponent;

/// This is a collection of items of one blueprint (UClass)
USTRUCT(BlueprintType, Category = "Item Collection")
struct FItemClassCollection
{
	GENERATED_USTRUCT_BODY()

	/// Defines the base class all items in this collection, initially defined by the Prototype
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item Collection")
	TSubclassOf<AItem> ItemClass;

	/// The actual item collection, one item row as you will
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item Collection")
	TArray<AItem*> ItemCollection;

	/* 
		The 'father' of the items in this collection because it defines the 'ItemClass' -
		this is either the first item that was added or one random if the original was removed 
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item Collection")
	AItem* Prototype;
};

//////////////////////////////////////////////////////////////////////////

// Inventory Delegates

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChangedSignature, UInventoryComponent*, inventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemAddedSignature, TSubclassOf<AItem>, baseClass, FItemClassCollection, classCollection, AItem*, item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemRemovedSignature, TSubclassOf<AItem>, baseClass, FItemClassCollection, classCollection, AItem*, item);

//////////////////////////////////////////////////////////////////////////

/**
*
*	The basic inventory component which can represent all kinds of stuff that can store sth in it.
*	
*	Supports filtering according to a specific item class, has events for general change, adding and removing items
*	and provides access to the actual content as well as utility functions to test if a item fits or is contained.
*	
*	The inventory can operate in 3 different behaviors:
*
*	Unlimited Mode, if no HasWeight or HasCount flag is set
*	Weight Mode, if only HasWeight is set
*	Count Mode, if only HasCount is set
*
*/
UCLASS(BlueprintType)
class CAEDE_API UInventoryComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	/// Adds a specific item to the inventory without any event - returns false if the item is too heavy
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Add A Item"))
	bool AddItem(AItem* item);

	/// Adds a specific item to the inventory, raising the OnItemPickedUp event - returns false if the item is too heavy
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Pickup A Item"))
	bool StoreItem(AItem* item);

	/// Returns a subset of items with a given class (an item class!) (1..n)
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Get Item(s) by Class"))
	bool GetItems(TSubclassOf<AItem> itemClass, FItemClassCollection& itemCollection);

	/// Returns a subset of items with a given class (an item class!) (1..n)
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Get Items Sorted By Class"))
	TArray<FItemClassCollection> GetItemsSortedByType();

	/// Removes an item form the inventory without any event (better use DropItem or GiveItem), returns false if the item is not in inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Remove A Item"))
	bool RemoveItem(AItem* item);

	/// Drops an item form the inventory, raising the OnItemDropped event
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Drop A Item"))
	bool DropItem(AItem* item);

	/// Removes an item form the inventory and gives it to another character (if possible)
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Give A Item To Someone"))
	bool GiveItem(AItem* item, ACaedeCharacter* character);

	/// Removes an item form the inventory and gives it to another inventory component
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Move Item To Other Component"))
	bool MoveItem(AItem* item, UInventoryComponent* inventory);

	/// Removes an item collection - item by item - form the inventory and gives it to another inventory component
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = ""))
	bool MoveItemCollection(TSubclassOf<AItem> itemClass, UInventoryComponent* inventory);

	/// Checks if this inventory contains the given item
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Check If Contains Item"))
	bool ContainsItem(AItem* item);

	/// Checks if a item would fit into this inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Fits Item Into Inventory"))
	bool FitsItem(AItem* item);

	/// Checks if this inventory has either no weight, no count or its 'InventoryMap' is empty
	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Is Empty"))
	bool IsEmpty();

	//////////////////////////////////////////////////////////////////////////

	/// Called when a item is added or removed from inventory
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnInventoryChangedSignature OnInventoryChanged;

	/// Gets called at any time a item is added (on Pickup, or Given) to this inventory
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnItemAddedSignature OnItemAdded;

	/// Gets called at any time a item is removed (on Drop, or Given Away) to this inventory
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnItemRemovedSignature OnItemRemoved;

	//////////////////////////////////////////////////////////////////////////

	/* 
		This class filters all items that will fit into the inventory - 
		make sure you make this either specific like 'AArmorItem' or 
		complete general like 'AItem'.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory")
	TSubclassOf<AItem> InventoryItemClass;

	/// If the 'HasWeight' flag is set the actual amount of weight in the inventory
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
	float CurrentInventoryWeight;
	
	/// If the 'HasWeight' flag is set the maximum store-able amount of weight in the inventory
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory")
	float MaxInventoryWeight;

	/// Indicates that the inventory will have a maximum amount of weight and won't take any more items if reached.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory")
	bool HasWeight;

	/// If the 'HasCount' flag is set the actual count of items in the inventory
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
	int32 CurrentInventoryCount;

	/// If the 'HasCount' flag is set the maximum store-able count of items in the inventory
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory")
	int32 MaxInventoryCount;

	/// Indicates that the inventory will have a maximum count of items and won't take any more if reached.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory")
	bool HasCount;

	/*
		The array with all the prototype items of one type (of blueprint!) - 
		To get all items use 'GetItems' for all of one Blueprint or use
		'GetItemsSortedByType' for all 'ItemClassCollection'
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
	TArray<AItem*> InventoryArray;

	/// The owning InventoryItem of this inventory
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
	class AInventoryItem* OwningItem;

	//////////////////////////////////////////////////////////////////////////

	/// The displayed UI name
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Localized, Category = "Inventory")
	FText InventoryName;

	//////////////////////////////////////////////////////////////////////////
	
	/// The internal inventory map with Class -> ClassCollection mapping aka THE REAL INVENTORY
	TMap<TSubclassOf<AItem>, FItemClassCollection> InventoryMap;

	/// Handles the correct update of the inventory map on add
	bool DeepAdd(UClass* itemClass, AItem* item);

	/// Handles the correct update of the inventory map on remove
	bool DeepRemove(UClass* itemClass, AItem* item);

	/// Handles the moving of items between inventory components
	bool DeepMove(AItem* item, UInventoryComponent* inventory);

	//////////////////////////////////////////////////////////////////////////

	virtual void InitializeComponent() override;

};
