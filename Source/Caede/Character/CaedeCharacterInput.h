

#pragma once

#include "GameFramework/Actor.h"
#include "CaedeCharacterInput.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API ACaedeCharacterInput : public AActor
{
	GENERATED_UCLASS_BODY()

	// Input Actions

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyInventoryDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyInventoryLeftHandDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyInventoryRightHandDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyInteractDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyDropDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeySitDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyBlockDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyCrouchDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyAttack1Down;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyAttack2Down;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyAttack3Down;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyAttack4Down;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyAttack5Down;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyRunDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyAbortDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeySelectDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyAgreeDown;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Input Actions")
	bool bKeyDisagreeDown;

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Character")
	ACaedeCharacter* OwningCharacter;

};
