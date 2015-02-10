// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"
#include "SlotContainerUserWidget.h"


void USlotContainerUserWidget::InitWidget(UPanelWidget* SlotContentDisplayPanel, UPanelWidget* ItemSlotsBox)
{
	if (SlotContentDisplayPanel == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("SlotContainer cannot initialize, SlotContentDisplayPanel is NULL!"));
		return;
	}
	if (ItemSlotsBox == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("SlotContainer cannot initialize, ItemSlotsBox is NULL!"));
		return;
	}

	UVerticalBox* vertical = Cast<UVerticalBox>(ItemSlotsBox);
	UHorizontalBox* horizontal = Cast<UHorizontalBox>(ItemSlotsBox);

	if (vertical == NULL && horizontal == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("The ItemSlotsBox must be a UVerticalBox or UHorizontalBox!"));
		return;
	}

	DisplayPanel = SlotContentDisplayPanel;
	SlotsBox = ItemSlotsBox;
}

bool USlotContainerUserWidget::WouldSlotFitIntoContainer(AItemSlot* SlotToCheck)
{
	for (TSubclassOf<AItemSlot> slotClass : AllowedSlotClasses)
	{
		if (SlotToCheck->GetClass()->IsChildOf(slotClass))
		{
			return true;
		}
	}
	return false;
}

bool USlotContainerUserWidget::CheckSlotCountNotReached()
{
	if (SlotsBox == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("SlotsBox for slot container is NULL - Not properly initialized!"));
		return false;
	}
	return (SlotsBox->GetChildrenCount()) < MaximumSlotCount;
}

//////////////////////////////////////////////////////////////////////////

bool USlotContainerUserWidget::AddItemSlotWidget(UItemSlotUserWidget* itemSlotWidget)
{
	if (SlotsBox == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("SlotsBox for slot container is NULL - Not properly initialized!"));
		return false;
	}
	if (DisplayPanel == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("DisplayPanel for slot container is NULL - Not properly initialized!"));
		return false;
	}
	itemSlotWidget->RemoveFromParent();
	SlotsBox->AddChild(itemSlotWidget);
	itemSlotWidget->DisplayPanel = DisplayPanel;
	itemSlotWidget->SlotContainer = this;
	return true;
}

bool USlotContainerUserWidget::RemoveItemSlotWidget(UItemSlotUserWidget* itemSlotWidget)
{
	if (SlotsBox == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("SlotsBox for slot container is NULL - Not properly initialized!"));
		return false;
	}
	if (itemSlotWidget->GetParent() == NULL)
	{
		return false;
	}
	itemSlotWidget->RemoveFromParent();
	if (itemSlotWidget->DisplayPanel != NULL)
	{
		itemSlotWidget->DisplayPanel->ClearChildren();
		itemSlotWidget->DisplayPanel = NULL;
	}
	itemSlotWidget->SlotContainer = NULL;
	return true;
}

bool USlotContainerUserWidget::SwitchItemSlotWidgets(UItemSlotUserWidget* thisWidget, USlotContainerUserWidget* thisContainer, 
	UItemSlotUserWidget* otherWidget, USlotContainerUserWidget* otherContainer)
{
	thisContainer->RemoveItemSlotWidget(thisWidget);
	otherContainer->RemoveItemSlotWidget(otherWidget);
	thisContainer->AddItemSlotWidget(otherWidget);
	otherContainer->AddItemSlotWidget(thisWidget);
	return true;
}
