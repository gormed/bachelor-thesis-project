// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"
#include "WeaponItemUserWidget.h"


UWeaponItemUserWidget::UWeaponItemUserWidget(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

void UWeaponItemUserWidget::InitWidget(AWeaponItem* item)
{
	if (item == NULL)
	{
		UE_LOG(LogCaedeWidget, Error, TEXT("Item for %s is invalid!"), *GetName());
		return;
	}
	ReferencedItem = item;
}


