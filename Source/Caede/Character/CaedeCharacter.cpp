

#include "Caede.h"
#include "Character/CaedeCharacter.h"

#include "Character/InventoryComponent.h"
#include "Character/EquipmentComponent.h"
#include "Character/InteractionComponent.h"
#include "Character/CharacterCameraComponent.h"
#include "Character/VitalityComponent.h"
#include "Character/SlotComponent.h"

#include "Items/BasicItem.h"
#include "Items/HerbItem.h"
#include "Items/WeaponItem.h"
#include "Items/QuestItem.h"

#include "Dialogs/DialogPawn.h"
#include "Dialogs/DialogComponent.h"

#include "Character/Slots/ItemSlot.h"
#include "Character/Slots/InventorySlot.h"
#include "Character/Slots/ArmorSlot.h"
#include "Character/Slots/WeaponSlot.h"

ACaedeCharacter::ACaedeCharacter(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

	//////////////////////////////////////////////////////////////////////////

	Equipment = PCIP.CreateDefaultSubobject<class UEquipmentComponent>(this, TEXT("Equipment"));

	//////////////////////////////////////////////////////////////////////////

	Interaction = PCIP.CreateDefaultSubobject<class UInteractionComponent>(this, TEXT("Interaction"));

	//////////////////////////////////////////////////////////////////////////

	CharacterCameraController = PCIP.CreateDefaultSubobject<class UCharacterCameraComponent>(this, TEXT("Character Camera Controller"));

	//////////////////////////////////////////////////////////////////////////

	Vitality = PCIP.CreateDefaultSubobject<class UVitalityComponent>(this, TEXT("Vitality"));

	//////////////////////////////////////////////////////////////////////////

	Slots = PCIP.CreateDefaultSubobject<class USlotComponent>(this, TEXT("Slots"));

	//////////////////////////////////////////////////////////////////////////

	CurrentInputMode = EPlayerInputMode::Move;
}

void ACaedeCharacter::BeginPlay()
{

	Interaction->OwningCharacter = this;

	Equipment->OwningCharacter = this;
	Equipment->BeginPlay();

	CharacterCameraController->OwningCharacter = this;
	CharacterCameraController->BeginPlay();

	Vitality->OwningCharacter = this;

	Slots->OwningCharacter = this;

	ACharacter::BeginPlay();
}

void ACaedeCharacter::Tick(float DeltaSeconds)
{
	if (Vitality != NULL && Vitality->IsActive())
	{
		TickVitality(DeltaSeconds, Vitality);
	}

	if (Interaction != NULL && Interaction->IsActive())
	{
		if (Interaction->CurrentDropItem != NULL)
		{
			Interaction->TickDropItem(DeltaSeconds);
		}
		if (Interaction->CurrentEquipItem != NULL)
		{
			Interaction->TickEquipItem(DeltaSeconds);
		}
		if (Interaction->CurrentPickupItem != NULL)
		{
			Interaction->TickPickupItem(DeltaSeconds);
		}
		if (Interaction->CurrentUseItem != NULL)
		{
			Interaction->TickUseItem(DeltaSeconds);
		}
		if (Interaction->CurrentStoreItem != NULL)
		{
			Interaction->TickStoreItem(DeltaSeconds);
		}
	}

	CharacterCameraController->TickCameraInterpolation(DeltaSeconds);

	ACharacter::Tick(DeltaSeconds);
}

bool ACaedeCharacter::CanJumpInternal_Implementation() const
{
	const bool bCanHoldToJumpHigher = (GetJumpMaxHoldTime() > 0.0f) && IsJumpProvidingForce();
	const UCharacterMovementComponent* cmc = GetCharacterMovement();

	return !bIsCrouched 
		&& cmc
		&& (cmc->IsMovingOnGround() || bCanHoldToJumpHigher)
		&& cmc->CanEverJump()
		&& !cmc->bWantsToCrouch
		&& CurrentInputMode == EPlayerInputMode::Move;
}

void ACaedeCharacter::SetInputMode(EPlayerInputMode::Type mode)
{
	if (CurrentInputMode != mode)
	{
		CurrentInputMode = mode;
	}
	else
	{
		UE_LOG(LogCaede, Log, TEXT("Changed to same input mode %d!"), (int32) mode);
	}
}

//////////////////////////////////////////////////////////////////////////

bool ACaedeCharacter::IsAggressive_Implementation()
{
	return Equipment->HandSelectedItem->IsA(AWeaponItem::StaticClass());
}

//
//bool ACaedeCharacter::IsAlive() const
//{
//	return Health->CurrentPawnHealth > 0;
//}

//////////////////////////////////////////////////////////////////////////
// Camera

bool ACaedeCharacter::EnableCameraControl(APlayerController* PlayerController)
{
	PlayerController->SetIgnoreLookInput(false);
	return true;
}

bool ACaedeCharacter::DisableCameraControl(APlayerController* PlayerController)
{
	PlayerController->SetIgnoreLookInput(true);
	return true;
}

#pragma region ///////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Dialogs

bool ACaedeCharacter::HasDialogPartner() const
{
	return CurrentPartner != NULL;
}

void ACaedeCharacter::StartDialog(ADialogPawn* Partner, bool Forced)
{
	if (Partner != NULL 
		&& Partner->CurrentDialogPartner == NULL 
		&& Partner->IsTalkable 
		&& Partner->DialogComponent->Dialogs.Num() > 0)
	{
		Partner->CurrentDialogPartner = this;
		//this->CharacterMovement->Deactivate();
		this->CurrentPartner = Partner;
		Partner->DialogComponent->OnPrepareDialog.Broadcast(Partner, Forced);
		Partner->DialogComponent->OnStartDialog.Broadcast(Partner, Forced);
		this->OnDialogStarted.Broadcast(this, Partner);
		UE_LOG(LogCaede, Display, TEXT("Dialog Started!"));
		return;
	}
	UE_LOG(LogCaede, Error, TEXT("CHAR000 - Failed to start dialog for Char %s! The both Partners could be unknown to eachother or there are no CurrentDialogs!"), *GetName());
}

void ACaedeCharacter::AbortDialog()
{
	if (CurrentPartner != NULL)
	{
		CurrentPartner->DialogComponent->FinishDialog(true);
		UE_LOG(LogCaede, Display, TEXT("Dialog Aborted!"));
		return;
	}
	UE_LOG(LogCaede, Error, TEXT("CHAR001 - Failed abort Dialog for %s! There is no partner!"), *GetName());
	return;
}

bool ACaedeCharacter::CanStartDialog_Implementation() const
{
	return CurrentPartner == NULL;
}

#pragma endregion Dialogs

bool ACaedeCharacter::CanStartInteract_Implementation() const
{
	return Interaction->CurrentUseItem == NULL && Interaction->CurrentPickupItem == NULL && Interaction->CurrentDropItem == NULL && Interaction->CurrentEquipItem == NULL;
}

bool ACaedeCharacter::TickVitality_Implementation(float DeltaSeconds, class UVitalityComponent* VitalityComponent)
{
	return true;
}

