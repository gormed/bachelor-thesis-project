

#pragma once

#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

//////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginHoverActorSignature, class AActor*, hovered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndHoverActorSignature, class AActor*, unhovered);

//////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType, Category = "Hand Slots")
namespace EHandSlot
{
	enum Type
	{
		LeftHand = 0,
		RightHand,
		BothHands,
		NoHands,
		Count,
		Invalid
	};
}

UENUM(BlueprintType, Category = "Item Interaction")
namespace EItemInteractionType
{
	enum Type
	{
		Pickup = 0,
		Drop,
		Equip,
		Store,
		Use,
		Count,
		Invalid
	};
}


/**
 * This component deals with items that can be interacted with in the world and from inventory.
 */
UCLASS(BlueprintType)
class CAEDE_API UInteractionComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Use", meta = (FriendlyName = "Start Use Item"))
	bool StartUseItem(class AItem* item);

	UFUNCTION(BlueprintCallable, Category = "Use", meta = (FriendlyName = "End Use Item"))
	bool EndUseItem(bool abort);

	bool TickUseItem(float deltaTime);

	//////////////////////////////////////////////////////////////////////////

	// add secondary functions support!

	// add store interaction!

	UFUNCTION(BlueprintCallable, Category = "Store", meta = (FriendlyName = "Can Store Item"))
	bool CanStoreItem(AItem* item);

	UFUNCTION(BlueprintCallable, Category = "Store", meta = (FriendlyName = "Start Store Item"))
	bool StartStoreItem(class AItem* item);

	UFUNCTION(BlueprintCallable, Category = "Store", meta = (FriendlyName = "End Store Item"))
	bool EndStoreItem(bool abort);

	bool TickStoreItem(float deltaTime);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Pickup", meta = (FriendlyName = "Can Pickup Item"))
	bool CanPickupItem(AItem* item);

	UFUNCTION(BlueprintCallable, Category = "Pickup", meta = (FriendlyName = "Start Pickup Item"))
	bool StartPickupItem(class AItem* item);
	
	UFUNCTION(BlueprintCallable, Category = "Pickup", meta = (FriendlyName = "End Pickup Item"))
	bool EndPickupItem(bool abort);

	bool TickPickupItem(float deltaTime);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Equip", meta = (FriendlyName = "Start Equip Item"))
	bool StartEquipItem(class AItem* item);

	UFUNCTION(BlueprintCallable, Category = "Equip", meta = (FriendlyName = "End Equip Item"))
	bool EndEquipItem(bool abort);

	bool TickEquipItem(float deltaTime);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Drop", meta = (FriendlyName = "Start Drop Item"))
	bool StartDropItem(class AItem* item);

	UFUNCTION(BlueprintCallable, Category = "Drop", meta = (FriendlyName = "End Drop Item"))
	bool EndDropItem(bool abort);

	bool TickDropItem(float deltaTime);

	//////////////////////////////////////////////////////////////////////////

	// Returns true if an interaction is running, also the current item - otherwise false and NULL
	UFUNCTION(BlueprintCallable, Category = "Current Interaction", meta = (FriendlyName = "Is Interacting With Item"))
	bool IsInteractingWithItem(AItem*& interactionItem, TEnumAsByte<EItemInteractionType::Type>& interactionType);

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Owner")
	class ACaedeCharacter* OwningCharacter;

	//////////////////////////////////////////////////////////////////////////

	// Current Interaction Item 

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Current Interaction")
	class AItem* CurrentUseItem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Current Interaction")
	class AItem* CurrentStoreItem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Current Interaction")
	class AItem* CurrentEquipItem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Current Interaction")
	class AItem* CurrentPickupItem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Current Interaction")
	class AItem* CurrentDropItem;

	//////////////////////////////////////////////////////////////////////////

	// Current interaction related slot or inventory

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Current Interaction")
	class AItemSlot* CurrentItemSlot;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Current Interaction")
	class UInventoryComponent* CurrentInventory;

	//////////////////////////////////////////////////////////////////////////

	// Hovering

	UFUNCTION(BlueprintCallable, Category = "Actor Hover", meta = (FriendlyName = "Get Current Hovered Actor"))
	bool GetCurrentHoveredActor(AActor*& hoveredActor);

	UFUNCTION(BlueprintCallable, Category = "Actor Hover", meta = (FriendlyName = "Fire Begin Hover Event"))
	void FireBeginHoverEvent(AActor* newHoverActor);

	UFUNCTION(BlueprintCallable, Category = "Actor Hover", meta = (FriendlyName = "Fire End Hover Event"))
	void FireEndHoverEvent(AActor* oldHoverActor);

	UPROPERTY(BlueprintAssignable, Category = "Actor Hover")
	FOnBeginHoverActorSignature OnBeginHoverActor;

	UPROPERTY(BlueprintAssignable, Category = "Actor Hover")
	FOnEndHoverActorSignature OnEndHoverActor;

private:

	class AActor* CurrentHoveredActor;

	bool PerformPickup(AItem* item);

	bool PerformUse(AItem* item);

	bool PerformEquip(AItem* item);

	bool PerformDrop(AItem* item);

	bool PerformStore(AItem* item);
};
