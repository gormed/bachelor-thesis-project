// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Dialogs/DialogPawn.h"
#include "DialogComponent.generated.h"

class UDialogState;
class UDialogComponent;


//////////////////////////////////////////////////////////////////////////
// EVENT SIGNATURES
//////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBeforeDialogTopicSignature, UDialog*, Dialog, ACaedeCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAfterDialogTopicSignature, UDialog*, Dialog, ACaedeCharacter*, Character);

//////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPrepareDialogSignature, ADialogPawn*, NPC, bool, Forced);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFinishDialogSignature, ADialogPawn*, NPC, bool, Forced);

//////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStartDialogSignature, ADialogPawn*, NPC, bool, Forced);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEndDialogSignature, ADialogPawn*, NPC, bool, Forced);

/**
 * A Dialog as representation of binary following states.
 * 
 * 
 */
UCLASS(BlueprintType)
class UDialogComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////
	/// FUNCTIONS
	//////////////////////////////////////////////////////////////////////////

	/// Create a new dialog component from persistence data, will be added to the dialog-list
	UFUNCTION(BlueprintCallable, Category = "Dialog Pawns", meta = (FriendlyName = "Create A New Dialog"))
	UDialog* CreateNewDialog(FString DialogName, FString DialogDescription, bool IsActive, bool IsFreeDialog, bool IsQuestDialog);

	//////////////////////////////////////////////////////////////////////////

	/// Indicates that the player said Y/N or Pro/Contra and goes to the next according state.
	/// Choose Pro = true for Positive or Pro = false for Negative next state
	UFUNCTION(BlueprintCallable, Category = "Dialog States", meta = (FriendlyName = "Next Dialog State"))
	UDialog* NextDialogState(bool Pro);

	/// End the current state if the stuff you wanted to do for this state is finished.
	/// Has to be called before the Next Dialog State function!!!
	/// Returns true if there is another state, false if the dialog ended
	UFUNCTION(BlueprintCallable, Category = "Dialog States", meta = (FriendlyName = "End Dialog State"))
	UDialog* EndDialogState(bool Forced);

	//////////////////////////////////////////////////////////////////////////

	/// Set a dialog active (can be set as active topic) or inactive (wont display and cannot be an active topic)
	UFUNCTION(BlueprintCallable, Category = "Dialog Pawns", meta = (FriendlyName = "Set Dialog Active"))
	void SetDialogActive(UDialog* Dialog, bool IsActive);

	//////////////////////////////////////////////////////////////////////////

	/// Checks if the pawn is talking to the given character.
	/// 
	/// Returns an invalid object if not talking to the 
	/// character or no dialog is currently active
	UFUNCTION(BlueprintCallable, Category = "Dialog Pawns", meta = (FriendlyName = "Is Talking To Me"))
	bool IsTalkingToMe(ACaedeCharacter* Me);

	//////////////////////////////////////////////////////////////////////////

	/// Check if the pawn is already talking to someone (maybe in MP)
	UFUNCTION(BlueprintCallable, Category = "Dialog Pawns", meta = (FriendlyName = "Has Dialog Partner"))
	bool HasDialogPartner();

	/// Check if the pawn has a active dialog to talk (also can check for quest and free dialogs)
	UFUNCTION(BlueprintCallable, Category = "Dialog Pawns", meta = (FriendlyName = "Has Active Dialog"))
	bool HasActiveDialog(bool checkQuest, bool checkFree);

	//////////////////////////////////////////////////////////////////////////
	/// DELEGATES
	//////////////////////////////////////////////////////////////////////////

	// Delegate Call Order:
	// 1. OnPrepareDialog -> OnStartDialog
	//		2. OnBeforeDialogTopic -> OnStartDialog
	//			3. OnEnterDialogState -> OnLeaveDialogState ...
	//		4. OnAfterDialogTopic -> OnBeforeDialogTopic ->  ...

	/// Is called before any other function if the player talkes to this pawn
	UPROPERTY(BlueprintAssignable)
	FPrepareDialogSignature OnPrepareDialog;

	/// Is called as the last function before the dialog finally ends
	UPROPERTY(BlueprintAssignable)
	FFinishDialogSignature OnFinishDialog;

	//////////////////////////////////////////////////////////////////////////

	/// Is called before a dialog topic starts
	UPROPERTY(BlueprintAssignable, Category = "Dialog Pawns")
	FBeforeDialogTopicSignature OnBeforeDialogTopic;

	/// Is called after a dialog topic ends or if the dialog is aborted
	UPROPERTY(BlueprintAssignable, Category = "Dialog Pawns")
	FAfterDialogTopicSignature OnAfterDialogTopic;

	//////////////////////////////////////////////////////////////////////////

	/// Is called after OnPrepareDialog Event
	UPROPERTY(BlueprintAssignable, Category = "Dialog Pawns Dialogs")
	FStartDialogSignature OnStartDialog;

	/// Is called before OnFinishDialog Event
	UPROPERTY(BlueprintAssignable, Category = "Dialog Pawns Dialogs")
	FEndDialogSignature OnEndDialog;

	//////////////////////////////////////////////////////////////////////////
	/// PROPERTIES
	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dialogs")
	TArray<UDialog*> Dialogs;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dialogs")
	ADialogPawn* OwningPawn;

	//////////////////////////////////////////////////////////////////////////
	/// INTERNAL
	//////////////////////////////////////////////////////////////////////////

	/// there can only be one active dialog at a time
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dialogs")
	UDialog* DialogTopic;

	//////////////////////////////////////////////////////////////////////////

	/// Finishes up the Dialog internally, could happen naturally or forced by player
	void FinishDialog(bool Forced);

	void FinishTopic(ACaedeCharacter* Partner, bool Forced);

	/// Set the current active dialog topic, must be active and not free, and in this components dialog-list
	UDialog* SetDialogTopic(UDialog* Dialog);

	/// This starts a dialog from a DialogPawn for a character.
	UDialog* StartDialogTopic();
};
