// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"
#include "Character/Slots/ItemSlot.h"
#include "Items/Item.h"
#include "ItemSlotUserWidget.h"


UItemSlotUserWidget::UItemSlotUserWidget(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

void UItemSlotUserWidget::InitWidget(AItemSlot* itemSlot)
{
	if (itemSlot == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("Item slot for %s is invalid!"), *GetName());
		return;
	}
	ReferencedSlot = itemSlot;

	if (ReferencedSlot->IsOccupied())
	{
		UUserWidget* widget = CreateSlotItemWidget(ReferencedSlot->SlotItem);
		if (widget != NULL)
		{
			SlotItemWidget = widget;
		}
	}

	ReferencedSlot->OnSlotItemInserted.AddUniqueDynamic(this, &UItemSlotUserWidget::OnSlotItemInserted_Internal);
	ReferencedSlot->OnSlotItemRemoved.AddUniqueDynamic(this, &UItemSlotUserWidget::OnSlotItemRemoved_Internal);
}

void UItemSlotUserWidget::OnSlotItemInserted_Internal(AItem* item)
{
	UUserWidget* widget = CreateSlotItemWidget(ReferencedSlot->SlotItem);
	if (widget != NULL)
	{
		SlotItemWidget = widget;
		OnSlotItemInserted(item, widget);
	}
}

void UItemSlotUserWidget::OnSlotItemRemoved_Internal(AItem* item)
{
	UUserWidget* widget = SlotItemWidget;
	if (widget != NULL)
	{
		SlotItemWidget = NULL;
		OnSlotItemRemoved(item, widget);
	}
}


