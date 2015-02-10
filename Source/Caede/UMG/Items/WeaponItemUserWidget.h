// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UMG/BaseWidgets/SlotBaseWidget.h"
#include "WeaponItemUserWidget.generated.h"

class AItem;
class AWeaponItem;

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API UWeaponItemUserWidget : public USlotBaseWidget
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item")
	AWeaponItem* ReferencedItem;

	UFUNCTION(BlueprintCallable, Category = "Item", meta = (FriendlyName = "Initialize Item Widget"))
	void InitWidget(AWeaponItem* item);
	
};
