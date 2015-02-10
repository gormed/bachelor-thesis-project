// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "InventoryItemRowUserWidget.generated.h"

class UInventoryComponent;

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API UInventoryItemRowUserWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item Row")
	TSubclassOf<AItem> ReferencedItemClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item Row")
	UInventoryComponent* ReferencedInventory;

	UFUNCTION(BlueprintCallable, Category = "Item Row", meta = (FriendlyName = "Initialize Item Row Widget"))
	void InitWidget(TSubclassOf<AItem> itemClass, UInventoryComponent* itemInventory);

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintImplementableEvent, Category = "Item Events", meta = (FriendlyName = "On Item Added"))
	void OnItemAdded(TSubclassOf<AItem> itemClass, AItem* item);

	UFUNCTION(BlueprintImplementableEvent, Category = "Item Events", meta = (FriendlyName = "On Item Removed"))
	void OnItemRemoved(TSubclassOf<AItem> itemClass, AItem* item);

};
