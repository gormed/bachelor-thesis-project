// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Character/EquipmentComponent.h"
#include "QuickInventoryBaseWidget.generated.h"

class AItem;
class AItemSlot;
class USlotComponent;
class UInventoryComponent;
class UInventoryBaseWidget;
class UItemSlotUserWidget;

/**
 *	The Quick inventory widget, which provides additional functionality 
 *	for the slots and the widgets which get inserted (Slots, Inventories).
 */
UCLASS(BlueprintType)
class CAEDE_API UQuickInventoryBaseWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Quick Inventory")
	USlotComponent* ReferencedSlotComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Quick Inventory")
	ACaedeHUD* PlayerHUD;

	UFUNCTION(BlueprintCallable, Category = "Quick Inventory", meta = (FriendlyName = "Initialize Quick Inventory Widget"))
	void InitWidget(USlotComponent* slotComponent);

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quick Inventory")
	TEnumAsByte<EInventoryMode::Type> CurrentInventoryMode;
	
	UFUNCTION(BlueprintCallable, Category = "Quick Inventory", meta = (FriendlyName = "Get Current Item Slots"))
	bool GetCurrentItemSlots(TArray<AItemSlot*>& slots);

	UFUNCTION(BlueprintCallable, Category = "Quick Inventory", meta = (FriendlyName = "Get Base Class From Mode"))
	bool GetBaseClassFromMode(UClass*& baseClass);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintImplementableEvent, Category = "Quick Inventory Events", meta = (FriendlyName = "Create Item Slot Widget"))
	UItemSlotUserWidget* CreateItemSlotWidget(AItemSlot* created, TSubclassOf<AItemSlot> baseType);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintImplementableEvent, Category = "Slot Events", meta = (FriendlyName = "On Slot Created"))
	void OnSlotCreated(AItemSlot* created, TSubclassOf<AItemSlot> createdBaseType, UItemSlotUserWidget* widget);

	UFUNCTION(BlueprintImplementableEvent, Category = "Slot Events", meta = (FriendlyName = "On Slot Removed"))
	void OnSlotRemoved(AItemSlot* removed, TSubclassOf<AItemSlot> removedBaseType, UItemSlotUserWidget* widget);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	void OnSlotCreated_Internal(AItemSlot* itemSlot, TSubclassOf<AItemSlot> baseType);

	UFUNCTION()
	void OnSlotRemoved_Internal(AItemSlot* itemSlot, TSubclassOf<AItemSlot> baseType);

protected:

	void SetupSlots();

	TMap<AItemSlot*, UItemSlotUserWidget*> ItemSlotWidgetsMap;
};
