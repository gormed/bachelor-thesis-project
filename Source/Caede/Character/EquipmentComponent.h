

#pragma once

#include "Map.h"

#include "Items/WeaponItem.h"

#include "Components/ActorComponent.h"

#include "Character/InteractionComponent.h"
#include "Character/InventoryComponent.h"

#include "EquipmentComponent.generated.h"


//////////////////////////////////////////////////////////////////////////

class AWeaponSlot;
class AItemSlot;
class AInventorySlot;
class AArmorSlot;
class AArmorItem;

//////////////////////////////////////////////////////////////////////////

/// The mode a inventory can have for UI related behavior
UENUM(BlueprintType, Category = "Inventory Mode")
namespace EInventoryMode
{
	enum Type
	{
		Item = 0,
		Inventory,
		Weapons,
		Armor,
		Count,
		Invalid
	};
}
//////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType, Category = "Navigation")
struct FNavigationSelection
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Navigation")
	TEnumAsByte<EInventoryMode::Type> NavigationMode;

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Navigation")
	AItemSlot* NavigationSlot;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Navigation")
	UInventoryComponent* NavigaionInventory;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Navigation")
	AItem* NavigationItem;

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Navigation")
	bool SlotChanged;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Navigation")
	bool ItemChanged;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Navigation")
	bool InventoryChanged;
};

//////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnNavigationChangedSignature, UUserWidget*, instigator, FNavigationSelection, oldSelection, FNavigationSelection, newSelection);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemSelectedSignature, class AItem*, item, TEnumAsByte<EHandSlot::Type>, handSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUnselectedSignature, class AItem*, item, TEnumAsByte<EHandSlot::Type>, handSlot);

//////////////////////////////////////////////////////////////////////////

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API UEquipmentComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Owner")
	ACaedeCharacter* OwningCharacter;

	//////////////////////////////////////////////////////////////////////////

#pragma region ///////////////////////////////

	bool CheckItem(AItem* item);

	template<class T>
	bool CheckSlot(AItemSlot* slot, TSubclassOf<AItemSlot> baseType, T*& outType);

	bool CheckSlots(TSubclassOf<AItemSlot> baseType);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Find Inventory For Item"))
	bool FindInventoryForItem(AItem* item, UInventoryComponent*& inventory);

	UFUNCTION(BlueprintCallable, Category = "", meta = (FriendlyName = "Is In Inventory"))
	bool IsInInventory(AItem* item, UInventoryComponent*& inventory);

	//////////////////////////////////////////////////////////////////////////

	// Handle Weapons separated from other items 

	UFUNCTION(BlueprintCallable, Category = "Weapons", meta = (FriendlyName = "Can Equip Weapon"))
	bool CanEquipWeapon(AWeaponItem* weapon);

	/// Equips a weapon to the next free slot automatically, returns the slot which was chosen
	UFUNCTION(BlueprintCallable, Category = "Weapons", meta = (FriendlyName = "Equip A Weapon To Free Slot"))
	int32 EquipWeapon(AWeaponItem* weapon);

	/// Equips a weapon to a given slot and unequips the last weapon in this slot, returns true if successful
	UFUNCTION(BlueprintCallable, Category = "Weapons", meta = (FriendlyName = "Equip A Weapon To Slot"))
	bool EquipWeaponToSlot(AWeaponItem* weapon, AItemSlot* slot);

	/// Finds a slot for the given weapon, returns true and the according slot if one exist - otherwise false and NULL
	UFUNCTION(BlueprintCallable, Category = "", meta = (FriendlyName = "Finds A Weapon Slot"))
	bool FindWeaponSlot(AWeaponItem* weapon, AItemSlot*& toSlot);

	/// Removes a weapon, returns true if successful
	UFUNCTION(BlueprintCallable, Category = "Weapons", meta = (FriendlyName = "Unequip A Weapon Directly"))
	bool UnequipWeapon(AWeaponItem* weapon);

	/// Removes a weapon from a given slot, returns true if successful
	UFUNCTION(BlueprintCallable, Category = "Weapons", meta = (FriendlyName = "Unequip A Weapon Slot"))
	AWeaponItem* UnequipWeaponFromSlot(AItemSlot* slot);

	//////////////////////////////////////////////////////////////////////////

	// Handle other items that can be equipped, like armor or a map

	UFUNCTION(BlueprintCallable, Category = "Armor", meta = (FriendlyName = "Can Equip Armor"))
	bool CanEquipArmor(AArmorItem* armor);

	UFUNCTION(BlueprintCallable, Category = "Armor", meta = (FriendlyName = "Finds A Armor Slot"))
	bool FindArmorSlot(AArmorItem* armor, AItemSlot*& toSlot);

	UFUNCTION(BlueprintCallable, Category = "Armor", meta = (FriendlyName = "Equip Armor To Slot"))
	bool EquipArmorToSlot(AArmorItem* armor, AItemSlot* slot);

	UFUNCTION(BlueprintCallable, Category = "Armor", meta = (FriendlyName = "Equip Armor"))
	bool EquipArmor(AArmorItem* armor);

	UFUNCTION(BlueprintCallable, Category = "Armor", meta = (FriendlyName = "Unequip Armor From Slot"))
	AArmorItem* UnequipArmorFromSlot(AItemSlot* slot);

	UFUNCTION(BlueprintCallable, Category = "Armor", meta = (FriendlyName = "Unequip Armor"))
	bool UnequipArmor(AArmorItem* armor);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Can Equip Inventory"))
	bool CanEquipInventory(AInventoryItem* inventory);

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Finds A Inventory Slot"))
	bool FindInventorySlot(AInventoryItem* inventory, AItemSlot*& toSlot);

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Equip Inventory To Slot"))
	bool EquipInventoryToSlot(AInventoryItem* inventory, AItemSlot* slot);

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Equip Inventory"))
	bool EquipInventory(AInventoryItem* inventory);

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Unequip Inventory From Slot"))
	AInventoryItem* UnequipInventoryFromSlot(AItemSlot* slot);

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (FriendlyName = "Unequip Inventory"))
	bool UnequipInventory(AInventoryItem* inventory);

#pragma endregion Equipment Functions

#pragma region ///////////////////////////////

	// Inventory navigation

	/// Describes which item types can be scrolled
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory Navigation")
	TEnumAsByte<EInventoryMode::Type> CurrentNavigationMode;

	/// Current Navigated Item Slot
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory Navigation")
	AItemSlot* CurrentNavigationSlot;

	/// The current inventory that can be navigated
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory Navigation")
	UInventoryComponent* CurrentNavigationInventory;

	/// Current Navigated Item
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory Navigation")
	AItem* CurrentNavigationItem;

	/// Navigates to a new selection if changes happened, raises the 'OnNavigationChanged' event
	UFUNCTION(BlueprintCallable, Category = "Inventory Navigation", meta = (FriendlyName = "Navigate"))
	bool Navigate(UUserWidget* instigator, TEnumAsByte<EInventoryMode::Type> mode, AItemSlot* slot, UInventoryComponent* inventory, AItem* item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Navigation", meta = (FriendlyName = "Clear Navigation"))
	void ClearNavigation();

	/// Gets called if the current navigation changed due 'Navigate'
	UPROPERTY(BlueprintAssignable, Category = "Inventory Navigation")
	FOnNavigationChangedSignature OnNavigationChanged;

	//////////////////////////////////////////////////////////////////////////

	// Item Hand Slot Selection
	
	/// Select a item directly (e.g. on pickup)
	UFUNCTION(BlueprintCallable, Category = "Hand Slots", meta = (FriendlyName = "Hand Select Item Directly"))
	AItem* SelectItem(AItem* item);

	/// Selects a item with the possibility to 
	UFUNCTION(BlueprintCallable, Category = "Hand Slots", meta = (FriendlyName = "Select Item Silently"))
	AItem* SelectItemSilent(AItem* item, bool unselect);

	/// Unselect a item directly (e.g. on drop)
	/// TODO: the unselected event was also meant to be used to perform an animation, so usually we do not 
	/// want the event to fire here. but due it is more confusing ATM, we do it nevertheless!
	UFUNCTION(BlueprintCallable, Category = "Hand Slots", meta = (FriendlyName = "Hand Unselect Item Directly"))
	AItem* UnselectItem(bool fireOnUselectItemEvent);

#pragma endregion Inventory Navigation

#pragma region ///////////////////////////////

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnItemSelectedSignature OnItemSelected;

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnItemUnselectedSignature OnItemUnselected;

	//////////////////////////////////////////////////////////////////////////

	// Hand Selection

	/// The currently selected item
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hand Selection")
	class AItem* HandSelectedItem;

	/// the current selected items according inventory
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hand Selection")
	UInventoryComponent* HandSelectedInventory;

	/// the current selected items according inventory
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hand Selection")
	AItemSlot* HandSelectedSlot;

#pragma endregion Hand Slots

	//////////////////////////////////////////////////////////////////////////

	void BeginPlay();

	virtual void BeginDestroy() override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:

	AItem* SwitchDirectly(AItem* item, bool unselect);

	AItem* DoSelection(AItem* item, bool unselect);

	AItem* DoUnselection(bool fireOnUnselectItemEvent);

	// links the given slot to an other slot (only works for shields and one hand weapons)
	void LinkWeaponSlot(AWeaponSlot* slot);


};
