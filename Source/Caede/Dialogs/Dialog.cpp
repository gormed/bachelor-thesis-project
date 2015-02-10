

#include "Caede.h"
#include "Dialogs/Dialog.h"
#include "Dialogs/DialogState.h"
#include "Dialogs/DialogComponent.h"


UDialog::UDialog(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	DialogName = "Unnamed Dialog";
	DialogDescription = "No Description";
	IsDialogActive = true;
	IsQuestDialog = false;
	IsFreeDialog = false;
	OwningComponent = NULL;
}

UDialogState* UDialog::CreateStartDialogState(FString stateName)
{
	if (StartDialogState != NULL)
	{
		UE_LOG(LogCaede, Error, TEXT("Start-State has already been created for %s!"), *GetName());
		return StartDialogState;
	}
	StartDialogState = NewNamedObject<UDialogState>(this, FName(*stateName));
	StartDialogState->IsPositiveState = false;
	StartDialogState->IsNegativeState = false;
	StartDialogState->OwningComponent = this;
	StartDialogState->ParentState = NULL;
	StartDialogState->DialogStateName = stateName;

	DialogStates.Add(StartDialogState);
	return StartDialogState;
}

void UDialog::RemoveStartDialogState(UDialogState* stateToRemove)
{
	if (stateToRemove != NULL && CurrentDialogState != stateToRemove)
	{
		DialogStates.Remove(stateToRemove);
	}
	UE_LOG(LogCaede, Error, TEXT("Failed to remove StartDialogState from %s!"), *GetName());
}
