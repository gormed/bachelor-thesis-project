// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "SlotContainerUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAEDE_API USlotContainerUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Slot Container")
	int32 MaximumSlotCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Slot Container")
	TArray<TSubclassOf<AItemSlot>> AllowedSlotClasses;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Slot Container")
	UPanelWidget* DisplayPanel;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Slot Container")
	UPanelWidget* SlotsBox;

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Slot Container", meta = (FriendlyName = "Init Slot Container Widget"))
	void InitWidget(UPanelWidget* SlotContentDisplayPanel, UPanelWidget* ItemSlotsBox);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Slot Container", meta = (FriendlyName = "Would Slot Fit Into Container"))
	bool WouldSlotFitIntoContainer(AItemSlot* SlotToCheck);

	UFUNCTION(BlueprintCallable, Category = "Slot Container", meta = (FriendlyName = "Check Slot Count Not Reached"))
	bool CheckSlotCountNotReached();

	//////////////////////////////////////////////////////////////////////////

	/// Adds a 'UItemSlotUserWidget' to the SlotsBox without(!) any checks
	UFUNCTION(BlueprintCallable, Category = "Slot Container", meta = (FriendlyName = "Add Item Slot Widget"))
	bool AddItemSlotWidget(UItemSlotUserWidget* itemSlotWidget);

	UFUNCTION(BlueprintCallable, Category = "Slot Container", meta = (FriendlyName = "Remove Item Slot Widget"))
	bool RemoveItemSlotWidget(UItemSlotUserWidget* itemSlotWidget);
	
	UFUNCTION(BlueprintCallable, Category = "Slot Container", meta = (FriendlyName = "Switch Item Slot Widgets"))
	bool SwitchItemSlotWidgets(UItemSlotUserWidget* thisWidget, USlotContainerUserWidget* thisContainer,
		UItemSlotUserWidget* otherWidget, USlotContainerUserWidget* otherContainer);
};
