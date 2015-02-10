
#pragma once

#include "GameFramework/Actor.h"
#include "Character/InteractionComponent.h"
#include "Item.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChangedOwnerSignature, class UInventoryComponent*, LastOwner, class UInventoryComponent*, NewOwner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDroppedSignature, class UInventoryComponent*, DroppedBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickedUpSignature, class UInventoryComponent*, PickeUpBy);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartItemEquipSignature, class ACaedeCharacter*, EquippedBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTickItemEquipSignature, float, DeltaTime, class ACaedeCharacter*, EquippedBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndItemEquipSignature, class ACaedeCharacter*, EquippedBy);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartItemUseSignature, class ACaedeCharacter*, UsedBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTickItemUseSignature, float, DeltaTime, class ACaedeCharacter*, UsedBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndItemUseSignature, class ACaedeCharacter*, UsedBy);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartItemStoreSignature, class ACaedeCharacter*, StoredBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTickItemStoreSignature, float, DeltaTime, class ACaedeCharacter*, StoredBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndItemStoreSignature, class ACaedeCharacter*, StoredBy);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartItemGrabSignature, class ACaedeCharacter*, InteractsWith);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTickItemGrabSignature, float, DeltaTime, class ACaedeCharacter*, InteractsWith);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndItemGrabSignature, class ACaedeCharacter*, InteractsWith);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartItemDropSignature, class ACaedeCharacter*, DroppedBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTickItemDropSignature, float, DeltaTime, class ACaedeCharacter*, DroppedBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndItemDropSignature, class ACaedeCharacter*, DroppedBy);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectedSignature, class ACaedeCharacter*, SelectedBy, TEnumAsByte<EHandSlot::Type>, handSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnselectedSignature, class ACaedeCharacter*, SelectedBy, TEnumAsByte<EHandSlot::Type>, handSlot);

//////////////////////////////////////////////////////////////////////////

// The different directions a item icon can change its content/weight value in UI
UENUM(BlueprintType, Category = "Item Icon Mode")
namespace EItemIconMode
{
	enum Type
	{
		Vertical,
		Horizontal,
		Count,
		Invalid
	};
}

//////////////////////////////////////////////////////////////////////////

/**
* A general item in Caede
*/
UCLASS(Meta = (ChildCanTick))
class CAEDE_API AItem : public AActor
{
	GENERATED_UCLASS_BODY()

public:

	//////////////////////////////////////////////////////////////////////////

	/// The value of the item in non capitalistic coins
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Basics")
	float		Value;

	/// The weight of the full item in kilogram
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Basics")
	float		FullWeight;

	/// The current weight of the item in kilogram
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Basics")
	float		Weight;

	/// The weight of the empty item in kilogram - implement the 'Actual Item Weight' function to specify 
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Basics")
	float		EmptyWeight;

	/// The internal name of the item
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Basics")
	FString		InternalItemName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Localized, Category = "Item Basics")
	FText		ItemDescription;

	/// The human readable name of the item
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Localized, Category = "Item Basics")
	FText		LocalizedItemName;

	/// Color to highlight - or whatever - the item
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Basics")
	FColor		IdenticationColor;

	//////////////////////////////////////////////////////////////////////////

	/// UI/HUD Icon Texture
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Basics|UI")
	UTexture2D* ItemIconTexture;

	/// The material used for displaying the icon of the item
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Basics|UI")
	TEnumAsByte<EItemIconMode::Type> ItemIconMode;

	//////////////////////////////////////////////////////////////////////////

	/// Defines how many or if any hands are used with this item
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Basics|Attachment")
	TEnumAsByte<EHandSlot::Type> UsesHands;

	/// Defines the socket the item will attach to on pickup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Basics|Attachment")
	FName PickupSocketName;

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Components")
	class USceneComponent* DefaultSceneRoot;

	/// The items mesh in the game
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Components")
	class UStaticMeshComponent* ItemMesh;

	/// Anchor for the info text
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Components")
	class USceneComponent* InfoAnchor;

	//////////////////////////////////////////////////////////////////////////

	/// Can the player drop the item
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Options")
	bool		CanBeDropped;

	/// Can a character or pawn equip the item
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Options")
	bool		CanBeEquipped;

	/// Can a character or pawn pickup the item
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Options")
	bool		CanBePickedUp;

	/// Can a character or pawn use the item
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Options")
	bool		CanBeUsed;

	/// Can a character or pawn store the item
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Options")
	bool		CanBeStored;

	//////////////////////////////////////////////////////////////////////////

	/// Should the item name be changeable
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Change Options")
	bool		CanChangeName;

	/// Should the item weight be changeable
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Change Options")
	bool		CanChangeWeight;

	/// Should the item value be changeable
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Change Options")
	bool		CanChangeValue;

	/// Should the item drop ability be changeable
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Change Options")
	bool		CanChangeDropping;
	
	/// Should the item equip ability be changeable
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Change Options")
	bool		CanChangeEquipping;

	/// Should the item pickup ability be changeable
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Change Options")
	bool		CanChangePickup;

	/// Should the item use ability be changeable
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Change Options")
	bool		CanChangeUsing;

	/// Should the item store ability be changeable
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Change Options")
	bool		CanChangeStore;

	//////////////////////////////////////////////////////////////////////////

	/// The current owner inventory of the item
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item Owner")
	class UInventoryComponent*	CurrentInventory;

	/// The current owner slot of the item
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item Owner")
	class AItemSlot*			CurrentItemSlot;

	/// The current owning character (if only in hands, this will be the only indicator)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item Owner")
	class ACaedeCharacter*		CurrentItemOwner;

	/// The original character owner of the item, defines in editor or on first pickup / equip
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Owner")
	class ACaedeCharacter*		OriginalItemOwner;

	//////////////////////////////////////////////////////////////////////////
	
	/// The amount left of this item in percent - returns 1 if not overridden in blueprint
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item Amount", meta = (FriendlyName = "Percentage Item Amount"))
	float PercentageAmount();

	/// The real weight of the item in kilogram - returns the Weight value if not overridden in blueprint
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item Amount", meta = (FriendlyName = "Actual Item Weight"))
	float ActualWeight();

	/// Checks if the item is empty, if possible (a bottle or sth like that) - returns false if not overridden in blueprint
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item Amount", meta = (FriendlyName = "Is Empty"))
	bool IsEmpty();

	// Provide the possibility to override the behavior of PercentageAmount, ActualWeight & IsEmpty also in C++
protected:
	virtual float PercentageAmount_Internal();
	virtual float ActualWeight_Internal();
	virtual bool IsEmpty_Internal();
public:

	//////////////////////////////////////////////////////////////////////////

	/// Change the name of the item (if possible)
	UFUNCTION(BlueprintCallable, Category = "Item Attributes", meta = (FriendlyName = "Change Item Name"))
	bool ChangeName(FText newItemName);

	/// Change the weight of the item (if possible)
	UFUNCTION(BlueprintCallable, Category = "Item Attributes", meta = (FriendlyName = "Change Item Weight"))
	bool ChangeWeight(float newItemWeight);

	/// Change the value of the item (if possible)
	UFUNCTION(BlueprintCallable, Category = "Item Attributes", meta = (FriendlyName = "Change Item Value"))
	bool ChangeValue(float newItemValue);

	/// Change the drop ability of the item (if possible)
	UFUNCTION(BlueprintCallable, Category = "Item Attributes", meta = (FriendlyName = "Change Drop Ability"))
	bool ChangeCanBeDropped(bool newCanBeDropped);

	/// Change the equip ability of the item (if possible)
	UFUNCTION(BlueprintCallable, Category = "Item Attributes", meta = (FriendlyName = "Change Equip Ability"))
	bool ChangeCanBeEquipped(bool newCanBeEqipped);

	/// Change the pickup ability of the item (if possible)
	UFUNCTION(BlueprintCallable, Category = "Item Attributes", meta = (FriendlyName = "Change Pickup Ability"))
	bool ChangeCanBePickedUp(bool newCanBePickedUp);

	/// Change the use ability of the item (if possible)
	UFUNCTION(BlueprintCallable, Category = "Item Attributes", meta = (FriendlyName = "Change Use Ability"))
	bool ChangeCanBeUsed(bool newCanBeUsed);

	/// Change the store ability of the item (if possible)
	UFUNCTION(BlueprintCallable, Category = "Item Attributes", meta = (FriendlyName = "Change Store Ability"))
	bool ChangeCanBeStored(bool newCanBeStored);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Item Collision", meta = (FriendlyName = "Toggle Collision"))
	void TogglePhyiscsAndCollision(bool enabled);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Item Socket", meta = (FriendlyName = "Attach Item To Actor"))
	bool AttachItem(AActor* parentActor, USceneComponent* parentMesh, FName socketName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Item Socket", meta = (FriendlyName = "Detach Item From Parent"))
	bool DetachItem();

	//////////////////////////////////////////////////////////////////////////

	// Inventory Component Events

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnItemChangedOwnerSignature	OnItemChangedOwner;

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnItemDroppedSignature			OnItemDropped;

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnItemPickedUpSignature		OnItemPickedUp;

	//////////////////////////////////////////////////////////////////////////

	// Selection Events

	UFUNCTION(BlueprintImplementableEvent, Category = "Item Events", meta = (FriendlyName = "On Hand Selected"))
	bool OnSelected(UEquipmentComponent* byEquipment);

	UFUNCTION(BlueprintImplementableEvent, Category = "Item Events", meta = (FriendlyName = "On Hand Unselected"))
	bool OnUnselected(UEquipmentComponent* byEquipment);

	/// called after the equipment component item-selection-event
	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnSelectedSignature			OnIsSelected;

	/// called after the equipment component item-unselection-event
	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnUnselectedSignature			OnIsUnselected;

	//////////////////////////////////////////////////////////////////////////

	// Interaction Component Events

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnStartItemEquipSignature		OnStartItemEquip;

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnTickItemEquipSignature		OnTickItemEquip;

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnEndItemEquipSignature		OnEndItemEquip;


	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnStartItemDropSignature		OnStartItemDrop;

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnTickItemDropSignature		OnTickItemDrop;

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnEndItemDropSignature			OnEndItemDrop;


	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnStartItemStoreSignature		OnStartItemStore;

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnTickItemStoreSignature		OnTickItemStore;

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnEndItemStoreSignature		OnEndItemStore;


	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnStartItemUseSignature		OnStartItemUse;
	
	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnTickItemUseSignature			OnTickItemUse;
	
	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnEndItemUseSignature			OnEndItemUse;


	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnStartItemGrabSignature		OnStartItemGrab;

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnTickItemGrabSignature		OnTickItemGrab;

	UPROPERTY(BlueprintAssignable, Category = "Item Events")
	FOnEndItemGrabSignature			OnEndItemGrab;

	//////////////////////////////////////////////////////////////////////////

	/// Apply this item to an owning inventory (if stored in an inventory)
	void SetItemInventory(class UInventoryComponent* itemOwner);

	/// Apply this item to an owning slot (if stored in a slot)
	void SetItemSlot(class AItemSlot* itemSlot);

	/// Apply this item to an owning character (if in hands or if in slot or inventory)
	virtual void SetItemOwner(class ACaedeCharacter* itemOwner);

	/// Set the item owner (slot or inventory) to NULL
	void SetItemFree(bool freeSlot, bool freeInventory, bool freeCharacter);

	//////////////////////////////////////////////////////////////////////////

	virtual void ReceiveDestroyed() override;

};
