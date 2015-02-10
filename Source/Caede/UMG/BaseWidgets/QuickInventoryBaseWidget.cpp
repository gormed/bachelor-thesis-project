// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"

#include "Character/Slots/ItemSlot.h"
#include "Character/Slots/ArmorSlot.h"
#include "Character/Slots/WeaponSlot.h"
#include "Character/Slots/InventorySlot.h"
#include "Character/SlotComponent.h"

#include "QuickInventoryBaseWidget.h"


UQuickInventoryBaseWidget::UQuickInventoryBaseWidget(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

void UQuickInventoryBaseWidget::InitWidget(USlotComponent* slotComponent)
{
	if (slotComponent == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("Slot component for %s is invalid!"), *GetName());
		return;
	}
	ReferencedSlotComponent = slotComponent;

	ReferencedSlotComponent->OnSlotCreated.AddUniqueDynamic(this, &UQuickInventoryBaseWidget::OnSlotCreated_Internal);
	ReferencedSlotComponent->OnSlotRemoved.AddUniqueDynamic(this, &UQuickInventoryBaseWidget::OnSlotRemoved_Internal);

	APlayerController* pc = GetOwningPlayer();
	if (pc != NULL)
	{
		PlayerHUD = Cast<ACaedeHUD>(pc->GetHUD());
	}

	SetupSlots();

}

bool UQuickInventoryBaseWidget::GetCurrentItemSlots(TArray<AItemSlot*>& slots)
{
	slots.Empty();

	switch (CurrentInventoryMode)
	{
	case EInventoryMode::Item:
		slots = ReferencedSlotComponent->GetSlots(AItemSlot::StaticClass());
		break;
	case EInventoryMode::Inventory:
		slots = ReferencedSlotComponent->GetSlots(AInventorySlot::StaticClass());
		break;
	case EInventoryMode::Weapons:
		slots = ReferencedSlotComponent->GetSlots(AWeaponSlot::StaticClass());
		break;
	case EInventoryMode::Armor:
		slots = ReferencedSlotComponent->GetSlots(AArmorSlot::StaticClass());
		break;
	default:
		UE_LOG(LogCaedeWidget, Error, TEXT("'Current Inventory Mode' for %s is invalid!"), *GetName());
		return false;
	}

	return true;
}

bool UQuickInventoryBaseWidget::GetBaseClassFromMode(UClass*& baseClass)
{
	switch (CurrentInventoryMode)
	{
	case EInventoryMode::Item:
		baseClass = AItemSlot::StaticClass();
		break;
	case EInventoryMode::Inventory:
		baseClass = AInventorySlot::StaticClass();
		break;
	case EInventoryMode::Weapons:
		baseClass = AWeaponSlot::StaticClass();
		break;
	case EInventoryMode::Armor:
		baseClass = AArmorSlot::StaticClass();
		break;
	default:
		UE_LOG(LogCaedeWidget, Error, TEXT("'Current Inventory Mode' for %s is invalid!"), *GetName());
		return false;
	}
	return true;
}

void UQuickInventoryBaseWidget::SetupSlots()
{
	TArray<AItemSlot*> itemSlots = ReferencedSlotComponent->GetSlots(AItemSlot::StaticClass());
	TArray<AItemSlot*> inventorySlots = ReferencedSlotComponent->GetSlots(AInventorySlot::StaticClass());
	TArray<AItemSlot*> armorSlots = ReferencedSlotComponent->GetSlots(AArmorSlot::StaticClass());
	TArray<AItemSlot*> weaponSlots = ReferencedSlotComponent->GetSlots(AWeaponSlot::StaticClass());

	ItemSlotWidgetsMap.Empty(itemSlots.Num() + inventorySlots.Num() + armorSlots.Num() + weaponSlots.Num());
	UItemSlotUserWidget* itemSlotWidget = NULL;

	for (AItemSlot* slot : itemSlots)
	{
		itemSlotWidget = CreateItemSlotWidget(slot, AItemSlot::StaticClass());
		if (itemSlotWidget != NULL)
		{
			ItemSlotWidgetsMap.Add(slot, itemSlotWidget);
		}
	}
	for (AItemSlot* slot : inventorySlots)
	{
		itemSlotWidget = CreateItemSlotWidget(slot, AInventorySlot::StaticClass());
		if (itemSlotWidget != NULL)
		{
			ItemSlotWidgetsMap.Add(slot, itemSlotWidget);
		}
	}
	for (AItemSlot* slot : armorSlots)
	{
		itemSlotWidget = CreateItemSlotWidget(slot, AArmorSlot::StaticClass());
		if (itemSlotWidget != NULL)
		{
			ItemSlotWidgetsMap.Add(slot, itemSlotWidget);
		}
	}
	for (AItemSlot* slot : weaponSlots)
	{
		itemSlotWidget = CreateItemSlotWidget(slot, AWeaponSlot::StaticClass());
		if (itemSlotWidget != NULL)
		{
			ItemSlotWidgetsMap.Add(slot, itemSlotWidget);
		}
	}
}

void UQuickInventoryBaseWidget::OnSlotCreated_Internal(AItemSlot* itemSlot, TSubclassOf<AItemSlot> baseType)
{
	UItemSlotUserWidget* widget = CreateItemSlotWidget(itemSlot, baseType);
	if (widget != NULL)
	{
		ItemSlotWidgetsMap.Add(itemSlot, widget);
		OnSlotCreated(itemSlot, baseType, widget);
	}
}

void UQuickInventoryBaseWidget::OnSlotRemoved_Internal(AItemSlot* itemSlot, TSubclassOf<AItemSlot> baseType)
{
	if (ItemSlotWidgetsMap.Contains(itemSlot) && ItemSlotWidgetsMap[itemSlot] != NULL)
	{
		UItemSlotUserWidget* widget = ItemSlotWidgetsMap[itemSlot];
		ItemSlotWidgetsMap.Remove(itemSlot);
		OnSlotRemoved(itemSlot, baseType, widget);
	}
}


