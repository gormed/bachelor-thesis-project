// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"
#include "AmorItemUserWidget.h"


UAmorItemUserWidget::UAmorItemUserWidget(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

void UAmorItemUserWidget::InitWidget(AArmorItem* item)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("Item for %s is invalid!"), *GetName());
		return;
	}
	ReferencedItem = item;
}


