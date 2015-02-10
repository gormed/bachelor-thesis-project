// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"

#include "Character/CaedeCharacter.h"
#include "Character/InteractionComponent.h"
#include "Character/EquipmentComponent.h"
#include "HUD/CaedeHUD.h"
#include "UMG/Interaction/HandItemUserWidget.h"
#include "UMG/Interaction/HoverUserWidget.h"

#include "InteractionBaseWidget.h"


UInteractionBaseWidget::UInteractionBaseWidget(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

void UInteractionBaseWidget::InitWidget(ACaedeCharacter* character)
{
	if (character == NULL)
	{
		UE_LOG(LogCaede, Error, TEXT("The referenced character for the interaction widget is invalid!"));
		return;
	}

	ReferencedCharacter = character;

	HandItemWidget = CreateHandItemWidget();
	HandItemWidget->InteractionWidget = this;
	HoverWidget = CreateHoverWidget();
	HoverWidget->InteractionWidget = this;

	ReferencedCharacter->Equipment->OnItemSelected.AddUniqueDynamic(this, &UInteractionBaseWidget::OnItemSelected_Internal);
	ReferencedCharacter->Equipment->OnItemUnselected.AddUniqueDynamic(this, &UInteractionBaseWidget::OnItemUnselected_Internal);

	ReferencedCharacter->Interaction->OnBeginHoverActor.AddUniqueDynamic(this, &UInteractionBaseWidget::OnBeginActorHover_Internal);
	ReferencedCharacter->Interaction->OnEndHoverActor.AddUniqueDynamic(this, &UInteractionBaseWidget::OnEndActorHover_Internal);

	APlayerController* pc = GetOwningPlayer();
	if (pc != NULL)
	{
		PlayerHUD = Cast<ACaedeHUD>(pc->GetHUD());
	}
}

void UInteractionBaseWidget::OnItemSelected_Internal(AItem* item, TEnumAsByte<EHandSlot::Type> handSlot)
{
	if (item != NULL)
	{
		OnItemSelected(item, handSlot);
	}
}

void UInteractionBaseWidget::OnItemUnselected_Internal(AItem* item, TEnumAsByte<EHandSlot::Type> handSlot)
{
	if (item != NULL)
	{
		OnItemUnselected(item, handSlot);
	}
}

void UInteractionBaseWidget::OnBeginActorHover_Internal(AActor* actor)
{
	if (actor != NULL)
	{
		OnBeginActorHover(actor);
	}
}

void UInteractionBaseWidget::OnEndActorHover_Internal(AActor* actor)
{
	if (actor != NULL)
	{
		OnEndActorHover(actor);
	}
}


