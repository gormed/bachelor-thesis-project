// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"

#include "Character/Slots/ItemSlot.h"
#include "Items/Item.h"

#include "SlotComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotCreatedSignature, class AItemSlot*, slot, TSubclassOf<AItemSlot>, baseType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotRemovedSignature, class AItemSlot*, slot, TSubclassOf<AItemSlot>, baseType);

/*
	Defines the initial slots at editor time and the specific base type slots at runtime.
*/
USTRUCT(BlueprintType)
struct FSlotDefinition
{
	GENERATED_USTRUCT_BODY()

	/// The basic type of the slot (Item, Weapon, Armor, Inventory)
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Slot Definition")
	TSubclassOf<AItemSlot> SlotBaseType;

	/// The specific slot definition (usually a BP inheriting AItemSlot or another base type)
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Slot Definition")
	TSubclassOf<AItemSlot> SlotSpecificType;

	/// The socket names, the slot will attach to (and also the item)
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Slot Definition")
	TArray<FName> SocketNames;

	/// The item to spawn into the slot on begin play
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Slot Definition")
	TSubclassOf<AItem> SlotItemType;

	/// The possible different instances of the slot (only relevant at game runtime)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Slot Occupiance")
	TArray<AItemSlot*> SlotIntstances;

	/// Checks if the given item is in one of the slot instances (for internal checks)
	bool ContainsItem(AItem* item, AItemSlot*& outSlot)
	{
		for (AItemSlot* slot : SlotIntstances)
		{
			if (slot->IsOccupied() && slot->SlotItem == item)
			{
				UE_LOG(LogCaedeSlot, Log, TEXT("Item %s found in slot %s!"), *item->InternalItemName, *slot->GetName());
				outSlot = slot;
				return true;
			}
		}
		UE_LOG(LogCaedeSlot, Log, TEXT("Item %s not found in slot!"), *item->InternalItemName);
		outSlot = NULL;
		return false;
	}

	/// Checks if the definition meets the inheritance criteria and nothing is NULL
	bool IsValidDefinition(const TSubclassOf<AItemSlot> slotSpecificType) const
	{
		return slotSpecificType != NULL 
			&& SlotBaseType != NULL
			&& SlotBaseType->IsChildOf(AItemSlot::StaticClass()) 
			&& slotSpecificType->IsChildOf(SlotBaseType);
	}
};

/**
 * Contains all references to ItemSlots and handles in editor creation of initial slots and all other slot specific behavior.
 */
UCLASS(BlueprintType)
class CAEDE_API USlotComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////

	/// The ownig item, if not owned by a CaedeCharacter
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Slot")
	class AItem* OwningItem;

	/// The owning character, if not owned by a Item
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item Slots")
	class ACaedeCharacter* OwningCharacter;

	/// List of initial created item slots and their items, socket names and other
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Slots")
	TArray<FSlotDefinition> InitialSlotDefinitions;

	/// The current available map key classes (AItemSlot, AWeaponSlot, AArmorSlot, AInventorySlot) of this slot component
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item Slots")
	TArray<UClass*> SlotBaseTypes;

	//////////////////////////////////////////////////////////////////////////

	/// Gets all slots of a specific base type (AItemSlot, AWeaponSlot, AArmorSlot, AInventorySlot)
	UFUNCTION(BlueprintCallable, Category = "Item Slots", meta = (FriendlyName = "Get Slots By Class"))
	TArray<AItemSlot*> GetSlots(UClass* baseType);

	/*
		Creates a new slot with a base type 'slotBaseType' (AItemSlot, AWeaponSlot, AArmorSlot, AInventorySlot),
		a specific type: 'slotSpecificType' (any Blueprint inheriting 'slotBaseType'),
		a list of socket names that will be checked on the parent mesh: 'socketNames',
		a parent actor for socket initialization: 'parentActor',
		and a possible socket item: 'socketItemClass' (any Blueprint inheriting AItem).
		
		Returns the created ItemSlot .
	*/
	UFUNCTION(BlueprintCallable, Category = "Item Slots", meta = (FriendlyName = "Create Slot From Definition"))
	AItemSlot* CreateSlotFromDefinition(UClass* slotBaseType, UClass* slotSpecificType, TArray<FName> socketNames, AActor* parentActor, UClass* socketItemClass);
	
	/*
		Removes a specific slot of a specific base type (AItemSlot, AWeaponSlot, AArmorSlot, AInventorySlot) 
		from the slot component, dropping the item in it!
	*/
	UFUNCTION(BlueprintCallable, Category = "Item Slots", meta = (FriendlyName = "Remove Existing Slot"))
	bool RemoveSlot(AItemSlot* remove, UClass* baseType);

	UPROPERTY(BlueprintAssignable, Category = "Item Slots")
	FOnSlotCreatedSignature OnSlotCreated;

	UPROPERTY(BlueprintAssignable, Category = "Item Slots")
	FOnSlotRemovedSignature OnSlotRemoved;

	//////////////////////////////////////////////////////////////////////////

	/// Disable the shortcuts on this component (for AI or in cut scenes?)
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Slot Shortcuts")
	bool bDisableSlotShortcuts;

	/// List of item on shortcuts
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item Slot Shortcuts")
	TArray<AItemSlot*> SlotItemShortcuts;

	/// List of shortcuts still available
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Slot Shortcuts")
	TArray<int32> AvailableShortcuts;

	//////////////////////////////////////////////////////////////////////////

	/// Gets a slot by the given int key
	UFUNCTION(BlueprintCallable, Category = "Item Slot Shortcuts", meta = (FriendlyName = "Get Slot From Shortcut"))
	bool GetSlotFromShortcut(int32 shortcut, AItemSlot*& slot);

	/// Binds a slot to a given int key, can force to unbind the old slot
	UFUNCTION(BlueprintCallable, Category = "Item Slot Shortcuts", meta = (FriendlyName = "Bind Slot To Shortcut"))
	bool BindSlotToShortcut(AItemSlot* itemSlot, int32 index, bool forceUnbindOld);

	/// Unbinds a shortcut and returns the possible bound item slot
	UFUNCTION(BlueprintCallable, Category = "Item Slot Shortcuts", meta = (FriendlyName = "Unbind Slot From Shortcut"))
	AItemSlot* UnbindSlotFromShortcut(int32 index);

	//////////////////////////////////////////////////////////////////////////

	/// Checks if a item is in one of the specified slot(s) and returns it and true if contained
	UFUNCTION(BlueprintCallable, Category = "Item Slot Functions", meta = (FriendlyName = "Contains Item"))
	bool ContainsItem(AItem* item, UClass* baseType, AItemSlot*& outSlot);

	/// Checks if a slot is present in a specific base type (AItemSlot, AWeaponSlot, AArmorSlot, AInventorySlot)
	UFUNCTION(BlueprintCallable, Category = "Item Slot Functions", meta = (FriendlyName = "Contains Slot"))
	bool ContainsSlot(AItemSlot* slot, UClass* baseType);

	/// Finds a slot of a base type (AItemSlot, AWeaponSlot, AArmorSlot, AInventorySlot) for the given item and returns it and true if found
	UFUNCTION(BlueprintCallable, Category = "Item Slot Functions", meta = (FriendlyName = "Find Slot For Item"))
	bool FindSlotForItem(AItem* item, UClass* baseType, AItemSlot*& foundSlot);

	//////////////////////////////////////////////////////////////////////////

	/*
		Gets all items in a base item slot type: 'baseType' (AItemSlot) 
		and a item type: 'baseItemType' (AItem)
	*/
	UFUNCTION(BlueprintCallable, Category = "Item Slot Functions", meta = (FriendlyName = "Get All Items Of Base Type"))
	TArray<AItem*> GetAllItemsOfBaseType(UClass* baseType, UClass* baseItemType);

	/*
		Gets the item of a base item slot type: 'baseType' (AItemSlot)
		and a item type: 'baseItemType' (AItem) and additionally checks 
		for a specific item type: 'specificItemType' (AItem)
	*/
	UFUNCTION(BlueprintCallable, Category = "Item Slot Functions", meta = (FriendlyName = "Get Specific Item"))
	AItem* GetSpecificItem(UClass* baseType, UClass* baseItemType, UClass* specificItemType);

	//////////////////////////////////////////////////////////////////////////

	/**
	* Starts gameplay for this component.
	* Requires component to be registered, and bWantsInitializeComponent to be TRUE.
	*/
	virtual void InitializeComponent() override;

	/**
	* Ends gameplay for this component.
	* Called from AActor::EndPlay only if bHasBeenInitialized is true
	*/
	virtual void UninitializeComponent() override;

	/// Map of all base types (AItemSlot, AWeaponSlot, AArmorSlot, AInventorySlot) and their according FSlotDefinition
	TMap<UClass*, FSlotDefinition> SlotMap;

	/// Map of all the shortcuts currently assigned
	TMap<int32, AItemSlot*> SlotShortcuts;

protected:

	/// Set of all free shortcuts, initially defined by the 'AvailableShortcuts'
	TSet<int32> FreeShortcuts;

	/// Handles specifics for the different base types (AItemSlot, AWeaponSlot, AArmorSlot, AInventorySlot)
	/// e.g. no armor shortcuts!
	void HandleSlotSpecifics(AItemSlot* slot, UClass* baseType);
};
