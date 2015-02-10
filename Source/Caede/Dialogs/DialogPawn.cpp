// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "Caede.h"

#include "HUD/CaedeHUD.h"

#include "Dialogs/DialogPawn.h"
#include "Dialogs/DialogComponent.h"
#include "Dialogs/DialogState.h"

#include "Character/CaedeCharacter.h"

#include "NPC/CaedeNPC.h"

ADialogPawn::ADialogPawn(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	CurrentDialogPartner = NULL;
	IsTalkable = true;
	DialogComponent = PCIP.CreateDefaultSubobject<UDialogComponent>(this, TEXT("Dialog Component"));

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ADialogPawn::BeginPlay()
{
	DialogComponent->OwningPawn = this;
	ACaedeNPC::BeginPlay();
}
