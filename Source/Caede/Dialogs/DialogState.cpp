// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "Caede.h"
#include "Dialogs/DialogState.h"
#include "Dialogs/DialogComponent.h"
#include "Dialogs/Dialog.h"

UDialogState::UDialogState(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	IsPositiveState = false;
	IsNegativeState = false;

	Entered = false;
	Left = false;

	NextPositiveState = NULL;
	NextNegativeState = NULL;
	ParentState = NULL;

	OwningComponent = NULL;
}

UDialogState* UDialogState::CreatePositiveFollowState(FString stateName)
{
	if (NextPositiveState != NULL)
		return NextPositiveState;

	NextPositiveState = NewNamedObject<UDialogState>(this, FName(*stateName));
	NextPositiveState->IsPositiveState = true;
	NextPositiveState->IsNegativeState = false;
	NextPositiveState->ParentState = this;
	NextPositiveState->OwningComponent = OwningComponent;
	NextPositiveState->DialogStateName = stateName;

	OwningComponent->DialogStates.Add(NextPositiveState);

	return NextPositiveState;
}

UDialogState* UDialogState::CreateNegativeFollowState(FString stateName)
{
	if (NextNegativeState != NULL)
		return NextNegativeState;

	NextNegativeState = NewNamedObject<UDialogState>(this, FName(*stateName));
	NextNegativeState->IsPositiveState = false;
	NextNegativeState->IsNegativeState = true;
	NextNegativeState->ParentState = this;
	NextNegativeState->OwningComponent = OwningComponent;
	NextPositiveState->DialogStateName = stateName;

	OwningComponent->DialogStates.Add(NextNegativeState);

	return NextNegativeState;
}
