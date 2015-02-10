// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NPC/CaedeNPC.h"
#include "DialogPawn.generated.h"

//////////////////////////////////////////////////////////////////////////

class ACaedeCharacter;
class UDialog;
class UDialogComponent;

//////////////////////////////////////////////////////////////////////////

/**
 * Someone or something that could be talked to ingame.
 */
UCLASS()
class ADialogPawn : public ACaedeNPC
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////

	/// Dialog Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogs")
	class UDialogComponent* DialogComponent;

	/// The character this pawn is talking to
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dialog Pawns")
	ACaedeCharacter* CurrentDialogPartner;

	//////////////////////////////////////////////////////////////////////////

	/// Can the player talk to me?
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Dialog Pawns")
	bool IsTalkable;

	//////////////////////////////////////////////////////////////////////////

	/// Set the owners for the components explicitly
	void BeginPlay() override;

};
