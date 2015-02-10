

#pragma once

#include "Object.h"
#include "Dialog.generated.h"

class UDialogState;
class UDialog;
class ACaedeCharacter;
class UDialogComponent;

#define HUD_DIALOG_TOPICS_UNSELECTED_COLOR FColor(75,75,255,255)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FStartDialogComponentSignature, UDialog*, Dialog, UDialogState*, StartState, ACaedeCharacter*, Character, bool, Forced);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FEndDialogComponentSignature, UDialog*, Dialog, UDialogState*, EndState, ACaedeCharacter*, Character, bool, Forced);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FDialogStateChangedSignature, UDialog*, Dialog, UDialogState*, LastState, UDialogState*, CurrentState, ACaedeCharacter*, Character, bool, Forced);

//////////////////////////////////////////////////////////////////////////
// PERSISTENCE STRUCT
//////////////////////////////////////////////////////////////////////////

/** Structure to store a dialog topic line */
USTRUCT(BlueprintType)
struct FDialogTopic
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog Topic")
	FString Topic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog Topic")
	class UDialog* Dialog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog Topic")
	FColor Color;

	FDialogTopic()
	{
		Topic = "NO TOPIC";
		Color = HUD_DIALOG_TOPICS_UNSELECTED_COLOR;
		Dialog = NULL;
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API UDialog : public UObject
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////
	/// PROPERTIES
	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dialog")
	FString DialogName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dialog")
	FString DialogDescription;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dialog")
	bool IsQuestDialog;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dialog")
	bool IsFreeDialog;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dialog")
	bool IsDialogActive;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Dialog")
	UDialogComponent* OwningComponent;

	//////////////////////////////////////////////////////////////////////////

	/// All states that can be reached through choices, there have to be at least one start state and one dialog state!
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialog States")
	TArray<UDialogState*> DialogStates;

	/// The entering state if the dialog starts, there have to be at least one start state and one dialog state!
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialog States")
	UDialogState* StartDialogState;

	/// The currently active state of the dialog (sub-conversation)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialog States")
	UDialogState* CurrentDialogState;

	//////////////////////////////////////////////////////////////////////////
	/// FUNCTIONS
	//////////////////////////////////////////////////////////////////////////

	/// Creates a possible start state of the dialog
	UFUNCTION(BlueprintCallable, Category = "Dialog Definitions", meta = (FriendlyName = "Create Start Dialog State"))
	UDialogState* CreateStartDialogState(FString stateName);

	/// Removes a possible start state of the dialog
	UFUNCTION(BlueprintCallable, Category = "Dialog Definitions", meta = (FriendlyName = "Removes Start Dialog State"))
	void RemoveStartDialogState(UDialogState* stateToRemove);

	//////////////////////////////////////////////////////////////////////////
	/// DELEGATES
	//////////////////////////////////////////////////////////////////////////

	/// Called if this dialog component is activated through a dialog-pawn from a player input
	UPROPERTY(BlueprintAssignable, Category = "Dialog Definitions")
	FStartDialogComponentSignature OnStartDialog;

	/// Called if the complete dialog comes to an end (player cancels the dialog or it ends normally)
	UPROPERTY(BlueprintAssignable, Category = "Dialog Definitions")
	FEndDialogComponentSignature OnEndDialog;

	/// Called if the current state changes (Y/N) on player input (a decision)
	UPROPERTY(BlueprintAssignable, Category = "Dialog Definitions")
	FDialogStateChangedSignature OnDialogStateChanged;
};
