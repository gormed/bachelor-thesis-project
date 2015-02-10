

#pragma once

#include "Components/ActorComponent.h"
#include "VitalityComponent.generated.h"

//////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDeathSignature, class ACaedeCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterExaustedSignature, class ACaedeCharacter*, Character);

//////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType, Category = "Statuses")
namespace EChacracterStatus
{
	enum Type
	{
		Health = 0,
		Stamina,

		Starvation,
		Thirst,

		Shit,
		Urin,

		Alcohol,
		Poison,
		Drug,

		Count,
		Invalid
	};
}

USTRUCT(BlueprintType)
struct FCharacterStatuses
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Valid Status?")
	bool Valid;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Primary Statuses")
	float Health;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Primary Statuses")
	float MaxHealth;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Primary Statuses")
	float Stamina;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Primary Statuses")
	float MaxStamina;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Secondary I Statuses")
	float Starvation;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Secondary I Statuses")
	float MaxStarvation;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Secondary I Statuses")
	float Thirst;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Secondary I Statuses")
	float MaxThirst;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Secondary II Statuses")
	float Urin;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Secondary II Statuses")
	float MaxUrin;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Secondary II Statuses")
	float Shit;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Secondary II Statuses")
	float MaxShit;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Tertiary Statuses")
	float Alcohol;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Tertiary Statuses")
	float MaxAlcohol;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Tertiary Statuses")
	float Poison;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Tertiary Statuses")
	float MaxPoison;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Tertiary Statuses")
	float Drug;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Tertiary Statuses")
	float DrugMax;

};

//////////////////////////////////////////////////////////////////////////

/**
 * This component manages vital parts of a character in caede as well as for the player and NPCs.
 */
UCLASS(BlueprintType)
class CAEDE_API UVitalityComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

#pragma region /////////////////////////////////

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Primary Statuses")
	float Health;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Primary Statuses")
	float MaxHealth;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Primary Statuses")
	float Stamina;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Primary Statuses")
	float MaxStamina;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Secondary I Statuses")
	float Starvation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Secondary I Statuses")
	float MaxStarvation;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Secondary I Statuses")
	float Thirst;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Secondary I Statuses")
	float MaxThirst;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Secondary II Statuses")
	float Urin;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Secondary II Statuses")
	float MaxUrin;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Secondary II Statuses")
	float Shit;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Secondary II Statuses")
	float MaxShit;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Tertiary Statuses")
	float Alcohol;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tertiary Statuses")
	float MaxAlcohol;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Tertiary Statuses")
	float Poison;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tertiary Statuses")
	float MaxPoison;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Tertiary Statuses")
	float Drug;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tertiary Statuses")
	float MaxDrug;

#pragma endregion Character Statuses

#pragma region /////////////////////////////////

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Statuses Flags")
	bool CanRevive;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Statuses Flags")
	bool HasHealth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Statuses Flags")
	bool HasStamina;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Statuses Flags")
	bool HasStarvation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Statuses Flags")
	bool HasThirst;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Statuses Flags")
	bool HasUrin;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Statuses Flags")
	bool HasShit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Statuses Flags")
	bool HasAlcohol;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Statuses Flags")
	bool HasPoison;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Statuses Flags")
	bool HasDrug;

#pragma endregion Statuses Flags

#pragma region /////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	// Primary

	/// Use this only in tick functions
	UFUNCTION(BlueprintCallable, Category = "Statuses|Primary", meta = (FriendlyName = "Regenerate Health"))
	void DoRegenerateHealth(float Amount);

	/// Use this only in tick functions
	UFUNCTION(BlueprintCallable, Category = "Statuses|Primary", meta = (FriendlyName = "Loose Health"))
	void DoLooseHealth(float Amount);

	/// Use this only in tick functions
	UFUNCTION(BlueprintCallable, Category = "Statuses|Primary", meta = (FriendlyName = "Regenerate Stamina"))
	void DoRegenerateStamina(float Amount);

	/// Use this only in tick functions
	UFUNCTION(BlueprintCallable, Category = "Statuses|Primary", meta = (FriendlyName = "Loose Stamina"))
	void DoLooseStamina(float Amount);

	// Secondary

	/// Use this only in tick functions
	UFUNCTION(BlueprintCallable, Category = "Statuses|Secondary", meta = (FriendlyName = "Eat"))
	void DoEat(float Amount);

	/// Use this only in tick functions
	UFUNCTION(BlueprintCallable, Category = "Statuses|Secondary", meta = (FriendlyName = "Drink"))
	void DoDrink(float Amount);

	/// Use this only in tick functions
	UFUNCTION(BlueprintCallable, Category = "Statuses|Secondary", meta = (FriendlyName = "Urinate"))
	void DoUrinate(float Amount);

	/// Use this only in tick functions
	UFUNCTION(BlueprintCallable, Category = "Statuses|Secondary", meta = (FriendlyName = "Shit"))
	void DoShit(float Amount);

	// Tertiary

	/// Use this only in tick functions
	UFUNCTION(BlueprintCallable, Category = "Statuses|Tertiary", meta = (FriendlyName = "Consume Alcohol"))
	void DoConsumeAlcohol(float Amount);

	/// Use this only in tick functions
	UFUNCTION(BlueprintCallable, Category = "Statuses|Tertiary", meta = (FriendlyName = "Consume Poison"))
	void DoConsumePoison(float Amount);

	/// Use this only in tick functions
	UFUNCTION(BlueprintCallable, Category = "Statuses|Tertiary", meta = (FriendlyName = "Consume Drug"))
	void DoConsumeDrug(float Amount);

#pragma endregion Status Manipulation Functions

#pragma region /////////////////////////////////

	/// Checks for character alive status. Checks if Health > 0
	UFUNCTION(BlueprintCallable, Category = "Statuses|Meta", meta = (FriendlyName = "Is Character Alive"))
	bool IsAlive();

	/// Revives the character
	UFUNCTION(BlueprintCallable, Category = "Statuses|Meta", meta = (FriendlyName = "Revive Character"))
	bool Revive();

	//////////////////////////////////////////////////////////////////////////

	/// Compare a status with a limit value
	UFUNCTION(BlueprintCallable, Category = "Statuses|Meta", meta = (FriendlyName = "Check For Lower Limit"))
	bool CheckForLowerLimit(TEnumAsByte<EChacracterStatus::Type> StatusType, float Limit);

	/// Compare a status with a limit value
	UFUNCTION(BlueprintCallable, Category = "Statuses|Meta", meta = (FriendlyName = "Check For Greater Limit"))
	bool CheckForGreaterLimit(TEnumAsByte<EChacracterStatus::Type> StatusType, float Limit);

#pragma endregion Statuses Meta

#pragma region /////////////////////////////////

	UPROPERTY(BlueprintAssignable, Category = "Statuses|Events")
	FOnCharacterDeathSignature OnCharacterDeath;

	UPROPERTY(BlueprintAssignable, Category = "Statuses|Events")
	FOnCharacterExaustedSignature OnCharacterExausted;

#pragma endregion Statuses Events

	ACaedeCharacter* OwningCharacter;

};
