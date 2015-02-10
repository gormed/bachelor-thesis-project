

#pragma once

#include "Components/ActorComponent.h"
#include "CharacterCameraComponent.generated.h"

USTRUCT(BlueprintType)
struct FCameraKeyPosition
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera")
	FString IdentifierKey;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera")
	FVector CameraBoomOrigin;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera")
	FRotator CameraBoomRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera")
	float CameraBoomLength;
};

USTRUCT(BlueprintType)
struct FCameraPostProcessingSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera")
	FString IdentifierKey;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera")
	float CameraPostProcessingInterpolationSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera")
	FPostProcessSettings CameraPostProcessingSettings;
};

/**
 * 
 */
UCLASS()
class CAEDE_API UCharacterCameraComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////

	// Camera Transformations

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera Transformations")
	TArray<FCameraKeyPosition> CameraKeyPositions;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera Transformations")
	FString StartKeyPosition;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera Transformations")
	FCameraKeyPosition LastKeyPosition;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera Transformations")
	FCameraKeyPosition CurrentKeyPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Transformations")
	bool StopTicking;

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Camera Transformations", meta = (FriendlyName = "New Target Camera Key Position"))
	void NewKeyPosition(FString IdentifierKey);

	//////////////////////////////////////////////////////////////////////////

	// Camera Post Processing

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera Post Processing")
	bool bDisablePostProcessingManagement;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera Post Processing")
	TArray<FCameraPostProcessingSettings> CameraPostProcessingCollection;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera Post Processing")
	bool bUseCustomCameraPostProcessingAtBeginPlay;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera Post Processing", meta = (editcondition = "bUseCustomCameraPostProcessingAtBeginPlay"))
	FString CustomStartKeyPostProcessing;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera Post Processing")
	FCameraPostProcessingSettings CurrentKeyPostProcessing;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Camera Post Processing")
	FCameraPostProcessingSettings LastKeyPostProcessing;

	//////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Camera Post Processing", meta = (FriendlyName = "New Key Post Process Settings"))
	void NewKeyPostProcessSettings(FString IdentifierKey);

	//////////////////////////////////////////////////////////////////////////

	// Internal

	ACaedeCharacter* OwningCharacter;

	TMap<FString, FCameraKeyPosition> CameraPositionsKeyMap;

	TMap<FString, FCameraPostProcessingSettings> CameraPostProcessingKeyMap;

	void TickCameraInterpolation(float DeltaTime);

	void BeginPlay();
};
