// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DialogState.generated.h"

//////////////////////////////////////////////////////////////////////////

class UDialogComponent;
class UDialogState;
class UDialog;

//////////////////////////////////////////////////////////////////////////

// Delegates for the different events for the interaction of states in a dialog

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEnterDialogStateSignature, UDialog*, OwningDialog, UDialogState*, DialogState, bool, Forced);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FLeaveDialogStateSignature, UDialog*, OwningDialog, UDialogState*, DialogState, bool, Forced);

//////////////////////////////////////////////////////////////////////////
// PERSISTENCE STRUCT
//////////////////////////////////////////////////////////////////////////

/** Structure to store a dialog state line */
USTRUCT(BlueprintType)
struct FDialogStateLine
{
	GENERATED_USTRUCT_BODY()

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog State")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog State")
	FString Subtitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog State")
	FString AudioID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subtitle")
	bool DisplayName;

	//////////////////////////////////////////////////////////////////////////

	FDialogStateLine()
	{
		Subtitle = "NONE";
		Name = "NONE";
		AudioID = "NONE";
		DisplayName = true;
	}

};

//////////////////////////////////////////////////////////////////////////

/**
 * A single state in a dialog (sub-converstation) to represent a choice of the player visually and auditive
 */
UCLASS(BlueprintType)
class UDialogState : public UObject
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////
	/// PROPERTIES
	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialog States")
	FString DialogStateName;

	/// Flag to signal a positive state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialog States")
	bool IsPositiveState;

	/// Flag to signal a negative state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialog States")
	bool IsNegativeState;

	/// The folowing state on a positive decision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dialog States")
	UDialogState* NextPositiveState;

	/// The folowing state on a negative decision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dialog States")
	UDialogState* NextNegativeState;

	/// The state before this
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dialog States")
	UDialogState* ParentState;

	/// The /see UDialogCompontent that owns this state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dialog States")
	UDialog* OwningComponent;

	//////////////////////////////////////////////////////////////////////////
	/// DELEGATES
	//////////////////////////////////////////////////////////////////////////

	/// Called if this state is entered (hook any animation, subtitles, audio, postprocessing, etc... here!) 
	UPROPERTY(BlueprintAssignable, Category="Dialog Definitions")
	FEnterDialogStateSignature OnEnterDialogState;

	/// Called if this state is left (hook any animation, subtitles, audio, postprocessing, etc... here!) 
	UPROPERTY(BlueprintAssignable, Category="Dialog Definitions")
	FLeaveDialogStateSignature OnLeaveDialogState;

	//////////////////////////////////////////////////////////////////////////
	/// FUNCTIONS
	//////////////////////////////////////////////////////////////////////////

	/// Creates or gets (if already created) the state that comes after this if the player chooses positive 
	UFUNCTION(BlueprintCallable, Category="Dialog Definitions", meta=(FriendlyName = "Create Positive Follow State"))
	UDialogState* CreatePositiveFollowState(FString stateName);

	/// Creates or gets (if already created) the state that comes after this if the player chooses negative 
	UFUNCTION(BlueprintCallable, Category="Dialog Definitions", meta=(FriendlyName = "Create Negative Follow State"))
	UDialogState* CreateNegativeFollowState(FString stateName);

	//////////////////////////////////////////////////////////////////////////
	/// Internals
	//////////////////////////////////////////////////////////////////////////

	/// Flag to signal that the state was entered (has to be reset from /see UDialogComponent)
	bool Entered;
	/// Flag to signal that the state was left (has to be reset from /see UDialogComponent)
	bool Left;
	
};
