// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ItemSlotUserWidget.generated.h"

class AItemSlot;
class AItem;
class USlotContainerUserWidget;

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API UItemSlotUserWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Slot")
	AItemSlot* ReferencedSlot;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Slot")
	UPanelWidget* DisplayPanel;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Slot")
	USlotContainerUserWidget* SlotContainer;

	UFUNCTION(BlueprintCallable, Category = "Slot", meta = (FriendlyName = "Initialize Slot Widget"))
	void InitWidget(AItemSlot* itemSlot);
	
	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Slot Item Widget")
	UUserWidget* SlotItemWidget;

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintImplementableEvent, Category = "Slot Item Events", meta = (FriendlyName = "Create Slot Item Widget"))
	UUserWidget* CreateSlotItemWidget(AItem* slotItem);

	UFUNCTION(BlueprintImplementableEvent, Category = "Slot Item Events", meta = (FriendlyName = "On Slot Item Inserted"))
	bool OnSlotItemInserted(AItem* item, UUserWidget* itemWidget);

	UFUNCTION(BlueprintImplementableEvent, Category = "Slot Item Events", meta = (FriendlyName = "On Slot Item Removed"))
	bool OnSlotItemRemoved(AItem* item, UUserWidget* itemWidget);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	void OnSlotItemInserted_Internal(AItem* item);

	UFUNCTION()
	void OnSlotItemRemoved_Internal(AItem* item);

	//////////////////////////////////////////////////////////////////////////


};
