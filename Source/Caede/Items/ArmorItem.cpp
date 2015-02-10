// Fill out your copyright notice in the Description page of Project Settings.

#include "Caede.h"
#include "ArmorItem.h"


AArmorItem::AArmorItem(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	CanBePickedUp = false;
	CanBeDropped = true;
	CanBeUsed = false;
	CanBeEquipped = true;
}


