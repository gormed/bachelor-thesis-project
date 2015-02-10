

#pragma once

#include "BasicItem.h"
#include "WeaponItem.generated.h"

UENUM(BlueprintType, Category = "Weapon")
namespace EWeaponType
{
	enum Type
	{
		OneHand,
		TwoHand,
		Shield,
		Bow,
		Knife,
		Quiver,
		Arrow,
		All,
		Count,
		Invalid
	};
}

USTRUCT(BlueprintType, Category = "Weapon")
struct FWeaponHitDesc
{
	GENERATED_USTRUCT_BODY()
	
	float Speed;
	FRotator ImpactAngle;

};

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API AWeaponItem : public AItem
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Weapon")
	TEnumAsByte<EWeaponType::Type> WeaponType;

	/// UI/HUD Details Texture
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item Basics|UI")
	UTexture2D* ItemDetailsTexture;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon", meta = (FriendlyName = "Calculate Damage"))
	float CalculateDamage(FWeaponHitDesc desc);

};