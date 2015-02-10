// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "Caede.h"

#include "Dialogs/DialogComponent.h"
#include "Dialogs/DialogState.h"
#include "Dialogs/Dialog.h"

#include "Character/CaedeCharacter.h"

UDialogComponent::UDialogComponent(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	DialogTopic = NULL;

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

//////////////////////////////////////////////////////////////////////////

UDialog* UDialogComponent::CreateNewDialog(FString DialogName, FString DialogDescription, bool IsDialogActive, bool IsFreeDialog, bool IsQuestDialog)
{
	UDialog* NewDialog = NewNamedObject<UDialog>(this, FName(*DialogName));
	NewDialog->DialogDescription = DialogDescription;
	NewDialog->DialogName = DialogName;
	NewDialog->IsDialogActive = IsDialogActive;
	NewDialog->IsFreeDialog = IsFreeDialog;
	NewDialog->IsQuestDialog = IsQuestDialog;
	NewDialog->OwningComponent = this;
	
	Dialogs.Add(NewDialog);

	UE_LOG(LogCaede, Display, TEXT("Created new dialog: %s! Flags: %d | %d | %d"),
		*NewDialog->DialogName, IsFreeDialog, IsQuestDialog, IsDialogActive);
	return NewDialog;
}
//////////////////////////////////////////////////////////////////////////

UDialog* UDialogComponent::NextDialogState(bool Pro)
{
	if (OwningPawn == NULL)
	{
		UE_LOG(LogCaede, Error, TEXT("%s has no OwningPawn!"), *GetName());
		return NULL;
	}

	if (DialogTopic == NULL || DialogTopic->CurrentDialogState == NULL)
	{
		UE_LOG(LogCaede, Error, TEXT("%s has either no CurrentDialog or State!"), *GetName());
		return NULL;
	}
	bool Forced = false;
	if (DialogTopic->CurrentDialogState->Left != true)
	{
		UE_LOG(LogCaede, Display, TEXT("Current state for pawn %s has not been left!"), *GetName());
		Forced = true;
		if (EndDialogState(Forced) == NULL)
		{
			return NULL;
		}
	}
	UDialogState* LastState = DialogTopic->CurrentDialogState;

	if (Pro)
	{
		// get the next positive state
		UDialogState* NextPositive = DialogTopic->CurrentDialogState->NextPositiveState;

		DialogTopic->CurrentDialogState = NextPositive;
		DialogTopic->CurrentDialogState->Entered = true;
		DialogTopic->CurrentDialogState->Left = false;
		NextPositive->OnEnterDialogState.Broadcast(DialogTopic, NextPositive, Forced);
		DialogTopic->OnDialogStateChanged.Broadcast(DialogTopic, LastState, NextPositive, OwningPawn->CurrentDialogPartner, Forced);
	}
	else
	{
		// get the next negative state
		UDialogState* NextNegative = DialogTopic->CurrentDialogState->NextNegativeState;

		DialogTopic->CurrentDialogState = NextNegative;
		DialogTopic->CurrentDialogState->Entered = true;
		DialogTopic->CurrentDialogState->Left = false;
		NextNegative->OnEnterDialogState.Broadcast(DialogTopic, NextNegative, Forced);
		DialogTopic->OnDialogStateChanged.Broadcast(DialogTopic, LastState, NextNegative, OwningPawn->CurrentDialogPartner, Forced);

	}
	return DialogTopic;
}

//////////////////////////////////////////////////////////////////////////

UDialog* UDialogComponent::EndDialogState(bool Forced)
{
	if (OwningPawn == NULL)
	{
		UE_LOG(LogCaede, Error, TEXT("%s has no OwningPawn!"), *GetName());
		return NULL;
	}

	if (DialogTopic == NULL || DialogTopic->CurrentDialogState == NULL)
	{
		UE_LOG(LogCaede, Error, TEXT("Current state for pawn %s has either no CurrentDialog or State!"), *GetName());
		return NULL;
	}
	if (DialogTopic->CurrentDialogState != NULL && DialogTopic->CurrentDialogState->Entered != true)
	{
		UE_LOG(LogCaede, Error, TEXT("Current state for pawn %s has not been entered!"), *GetName());
		return NULL;
	}

	UDialogState* LastState = DialogTopic->CurrentDialogState;
	LastState->Left = true;
	LastState->Entered = false;
	LastState->OnLeaveDialogState.Broadcast(DialogTopic, LastState, Forced);

	// if we go to an ending state (a state without ANY childs)
	if (LastState->NextPositiveState == NULL
		|| LastState->NextNegativeState == NULL)
	{
		if (!HasActiveDialog(false, false))
		{
			FinishDialog(Forced);
		}
		else
		{
			FinishTopic(OwningPawn->CurrentDialogPartner, Forced);
		}
		return NULL;
	}
	return DialogTopic;
}

//////////////////////////////////////////////////////////////////////////

void UDialogComponent::SetDialogActive(UDialog* Dialog, bool IsActive)
{
	if (Dialog != NULL
		&& Dialogs.Contains(Dialog))
	{
		Dialog->IsDialogActive = IsActive;
		return;
	}
	UE_LOG(LogCaede, Error, TEXT("Failed to set dialog active for %s! The dialog is NULL or not in the dialogs list!"), *GetName());
}


//////////////////////////////////////////////////////////////////////////

bool UDialogComponent::IsTalkingToMe(ACaedeCharacter* Me)
{
	UE_LOG(LogCaede, Warning, TEXT("Calling at wrong position? Failed to check IsTalingToMe for %s!"), *GetName());
	if (Me == NULL || OwningPawn == NULL || OwningPawn->CurrentDialogPartner == NULL)
	{
		UE_LOG(LogCaede, Error, TEXT("Calling at wrong position? Failed to check IsTalingToMe for %s!"), *GetName());
		return false;
	}
	return Me == OwningPawn->CurrentDialogPartner;
}

//////////////////////////////////////////////////////////////////////////

bool UDialogComponent::HasDialogPartner()
{
	return OwningPawn->CurrentDialogPartner != NULL;
}

bool UDialogComponent::HasActiveDialog(bool checkQuest, bool checkFree)
{
	for (int32 i = 0; i < Dialogs.Num(); i++)
	{
		if (Dialogs[i]->IsDialogActive 
			&& Dialogs[i]->IsFreeDialog == checkFree
			&& Dialogs[i]->IsQuestDialog == checkQuest)
		{
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

void UDialogComponent::FinishDialog(bool Forced)
{
	if (OwningPawn != NULL && OwningPawn->CurrentDialogPartner != NULL)
	{
		// Store partner
		ACaedeCharacter* Partner = OwningPawn->CurrentDialogPartner;

		FinishTopic(Partner, Forced);

		Partner->OnDialogEnded.Broadcast(Partner, this->OwningPawn);

		Partner->CurrentPartner = NULL;
		OwningPawn->CurrentDialogPartner = NULL;

		// Log
		UE_LOG(LogCaede, Display, TEXT("Cleaned up dialog for %s!"), *GetName());
	}
	else
	{
		UE_LOG(LogCaede, Error, TEXT("Cannot finish Dialog for %s! No current dialog or no partner!"), *GetName());
	}
}

//////////////////////////////////////////////////////////////////////////

UDialog* UDialogComponent::SetDialogTopic(UDialog* Dialog)
{
	if ((DialogTopic == NULL && Dialog != NULL && Dialog->CurrentDialogState == NULL) ||

		(Dialog != NULL && DialogTopic != Dialog 
		&& DialogTopic->CurrentDialogState == NULL 
		&& Dialog->CurrentDialogState == NULL))
	{
		if (Dialogs.Contains(Dialog))
		{
			DialogTopic = Dialog;
			return DialogTopic;
		}
		UE_LOG(LogCaede, Error,
			TEXT("Failed to set active dialog for %s! Dialog not in the dialogs list, please use SetDialogActive or create the dialog as active!"), *GetName());
		return NULL;
	}
	UE_LOG(LogCaede, Error, TEXT("Failed to set active dialog for %s! Maybe there is already a active dialog, the dialog could be invalid or the dialog could have been started!"), *GetName());
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

UDialog* UDialogComponent::StartDialogTopic()
{
	// if valid dialog and dialog not started yet or ended
	if (OwningPawn != NULL
		&& OwningPawn->CurrentDialogPartner != NULL
		&& OwningPawn->CurrentDialogPartner->CurrentPartner != NULL
		&& DialogTopic != NULL)
	{
		OnBeforeDialogTopic.Broadcast(DialogTopic, OwningPawn->CurrentDialogPartner);
		if (DialogTopic->StartDialogState != NULL)
		{
			DialogTopic->StartDialogState->Entered = true;
			DialogTopic->StartDialogState->Left = false;
			DialogTopic->OnStartDialog.Broadcast(DialogTopic, DialogTopic->StartDialogState, OwningPawn->CurrentDialogPartner, false);
			DialogTopic->CurrentDialogState = DialogTopic->StartDialogState;
			DialogTopic->CurrentDialogState->OnEnterDialogState.Broadcast(DialogTopic, DialogTopic->CurrentDialogState, false);
			DialogTopic->OnDialogStateChanged.Broadcast(
				DialogTopic, NULL, DialogTopic->CurrentDialogState, OwningPawn->CurrentDialogPartner, false);
			OwningPawn->CurrentDialogPartner->OnDialogTopicSelected.Broadcast(this);
			UE_LOG(LogCaede, Display, TEXT("Started DialogTopic %s with pawn %s"), *DialogTopic->DialogName, *GetName());
			return DialogTopic;
		}
		UE_LOG(LogCaede, Error, TEXT("Current Dialog %s for %s has no start state!"), *DialogTopic->DialogName, *GetName());
		FinishDialog(false);
		return NULL;
	}
	UE_LOG(LogCaede, Error, TEXT("%s has either no OwningPawn, CurrentDialogPartner or no CurrentDialog!"), *GetName());
	FinishDialog(false);
	return NULL;
}

void UDialogComponent::FinishTopic(ACaedeCharacter* Partner, bool Forced)
{
	// Cleanup
	//CurrentDialogPartner->CharacterMovement->Activate();
	// If dialog is running
	if (DialogTopic != NULL)
	{
		UDialogState* LastState = DialogTopic->CurrentDialogState;
		// Singal the state as left!
		DialogTopic->CurrentDialogState = NULL;
		// broadcast the ending to events
		DialogTopic->OnEndDialog.Broadcast(DialogTopic, LastState, Partner, Forced);
		OnAfterDialogTopic.Broadcast(DialogTopic, Partner);
	}
	// broadcast the conversation-end
	OnEndDialog.Broadcast(OwningPawn, Forced);
	OnFinishDialog.Broadcast(OwningPawn, Forced);
	Partner->OnDialogTopicEnded.Broadcast(this, Forced);
	DialogTopic = NULL;
}
