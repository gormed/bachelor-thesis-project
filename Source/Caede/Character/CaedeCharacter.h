

#pragma once

#include "GameFramework/Character.h"
#include "CaedeCharacter.generated.h"

//LOG_SCOPE_VERBOSITY_OVERRIDE(LogCaede, ELogVerbosity::Display);

class ADialogPawn;
class UDialogComponent;
class ACaedeCharacter;

//////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogTopicSelectedSignature, UDialogComponent*, SelectedDialog);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogTopicEndedSignature, UDialogComponent*, EndedDialog, bool, Forced);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogTopicChangedSignature, UDialogComponent*, CurrentDialog, bool, Forced);

//////////////////////////////////////////////////////////////////////////

// Used to prepare and postpare settings for the char if in dialog
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogStartedSignature, ACaedeCharacter*, Character, ADialogPawn*, Partner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogEndedSignature, ACaedeCharacter*, Character, ADialogPawn*, Partner);

//////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType, Category = "Input Mode")
namespace EPlayerInputMode
{
	enum Type
	{
		Move = 0,
		Inventory,
		Fight,
		Sit,
		Dialog,
		Count,
		Invalid
	};
}

/**
 * The caede game main character for the player and NPCs.
 */
UCLASS()
class CAEDE_API ACaedeCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

#pragma region /////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	// Basic

	/*
	* Check if the character is aggressive (can be overridden - currently checks 'if character has any weapon in hands')
	* Used for NPC behavior only!
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Meta|Basic", meta = (FriendlyName = "Is Character Aggressive"))
	bool IsAggressive();

	//////////////////////////////////////////////////////////////////////////

	// Input Mode

	/// Set the current player input mode
	UFUNCTION(BlueprintCallable, Category = "Meta|Input Mode", meta = (FriendlyName = "Set Input Mode"))
	void SetInputMode(EPlayerInputMode::Type mode);

	/// The current input mode, can be set with 'Set Input Mode'
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Meta|Input Mode")
	TEnumAsByte<EPlayerInputMode::Type> CurrentInputMode;

#pragma endregion Meta

#pragma region /////////////////////////////////


	/// Slot Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Slots")
	class USlotComponent* Slots;

#pragma endregion Slots

#pragma region /////////////////////////////////

	/// Equipment Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Equipment")
	class UEquipmentComponent* Equipment;

#pragma endregion Equipment


#pragma region /////////////////////////////////


#pragma endregion Input Mode

#pragma region /////////////////////////////////

	/// Vitality Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Vitality")
	class UVitalityComponent* Vitality;

	UFUNCTION(BlueprintNativeEvent, Category = "Components|Vitality", meta = (FriendlyName = "Tick Vitality"))
	bool TickVitality(float DeltaSeconds, class UVitalityComponent* VitalityComponent);

#pragma endregion Vitality

#pragma region /////////////////////////////////

	/// The current dialog partner
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogs")
	ADialogPawn* CurrentPartner;

	//////////////////////////////////////////////////////////////////////////

	/// Checks if this character has a partner pawn
	UFUNCTION(BlueprintCallable, Category = "Dialogs", meta = (FriendlyName = "Has Dialog Partner"))
	bool HasDialogPartner() const;

	/// Starts a dialog with a given pawn.
	UFUNCTION(BlueprintCallable, Category = "Dialogs", meta = (FriendlyName = "Start A Dialog With Pawn"))
	void StartDialog(ADialogPawn* Partner, bool Forced);

	/// Aborts a dialog for this character with this pawn.
	UFUNCTION(BlueprintCallable, Category = "Dialogs", meta = (FriendlyName = "Abort Current Dialog"))
	void AbortDialog();

	/// Checks if this character can start a dialog (Can be overridden by Blueprint!)
	UFUNCTION(BlueprintNativeEvent, Category = "Dialogs", meta = (FriendlyName = "Can Start Dialog"))
	bool CanStartDialog() const;

	//////////////////////////////////////////////////////////////////////////

	// Delegates

	/// Is raised if the player selects a dialog topic of a pawn
	UPROPERTY(BlueprintAssignable, Category = "Dialogs")
	FDialogTopicSelectedSignature OnDialogTopicSelected;

	UPROPERTY(BlueprintAssignable, Category = "Dialogs")
	FDialogTopicEndedSignature OnDialogTopicEnded;

	UPROPERTY(BlueprintAssignable, Category = "Dialogs")
	FDialogStartedSignature OnDialogStarted;

	UPROPERTY(BlueprintAssignable, Category = "Dialogs")
	FDialogEndedSignature OnDialogEnded;

#pragma endregion Dialogs

#pragma region /////////////////////////////////

	/// Interaction Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Interaction")
	class UInteractionComponent* Interaction;

	/// Checks if the character can interact (Can be overridden by Blueprint!)
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction", meta = (FriendlyName = "Can Start Interact"))
	bool CanStartInteract() const;

#pragma endregion Interaction

#pragma region /////////////////////////////////

	/// Character Camera Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera")
	class UCharacterCameraComponent* CharacterCameraController;

	/// Get the camera of the char in c++ (Needs implementation in BP)
	UFUNCTION(BlueprintImplementableEvent, Category = "Components|Camera", meta = (FriendlyName = "Get Camera"))
	class UCameraComponent* GetCamera();

	/// Get the camera of the char in c++ (Needs implementation in BP)
	UFUNCTION(BlueprintImplementableEvent, Category = "Components|Camera", meta = (FriendlyName = "Get Camera Boom"))
	class USpringArmComponent* GetCameraBoom();

	/// Interpolates the new camera transform from the given current relative transform (Needs implementation in BP)
	UFUNCTION(BlueprintImplementableEvent, Category = "Components|Character Camera", meta = (FriendlyName = "Interpolate Camera Transform"))
	bool InterpolateCameraTransform(float DeltaTime, class USpringArmComponent* CameraBoom, class UCameraComponent* Camera);

	/// Interpolates the new post processing settings from the current settings (Needs implementation in BP)
	UFUNCTION(BlueprintImplementableEvent, Category = "Components|Character Camera", meta = (FriendlyName = "Interpolate Camera Post Processing"))
	bool InterpolateCameraPostProcessing(float DeltaTime, class UCameraComponent* Camera);

	/// Enable Camera Control
	UFUNCTION(BlueprintCallable, Category = "Components|Camera", meta = (FriendlyName = "Enable The Characters Camera"))
	bool EnableCameraControl(APlayerController* PlayerController);

	/// Diable Camera Control
	UFUNCTION(BlueprintCallable, Category = "Components|Camera", meta = (FriendlyName = "Disable The Characters Camera"))
	bool DisableCameraControl(APlayerController* PlayerController);

#pragma endregion Camera

	//////////////////////////////////////////////////////////////////////////

	/// Set the owners for the components explicitly
	void BeginPlay() override;

	/// Ticks the characters threads
	void Tick(float DeltaSeconds) override;

	virtual bool CanJumpInternal_Implementation() const override;
};
