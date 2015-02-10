// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UMG/BaseWidgets/SlotBaseWidget.h"
#include "AmorItemUserWidget.generated.h"

class AItem;
class AArmorItem;

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API UAmorItemUserWidget : public USlotBaseWidget
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item")
	AArmorItem* ReferencedItem;

	UFUNCTION(BlueprintCallable, Category = "Item", meta = (FriendlyName = "Initialize Item Widget"))
	void InitWidget(AArmorItem* item);
	
};
