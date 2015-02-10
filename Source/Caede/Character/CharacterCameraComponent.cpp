

#include "Caede.h"

#include "Character/CharacterCameraComponent.h"
#include "Character/CaedeCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


UCharacterCameraComponent::UCharacterCameraComponent(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostPhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	StartKeyPosition = "Move";
}

//////////////////////////////////////////////////////////////////////////

void UCharacterCameraComponent::TickCameraInterpolation(float DeltaTime)
{
	if (StopTicking) return;

	UCameraComponent* camera = OwningCharacter->GetCamera();
	USpringArmComponent* cameraBoom = OwningCharacter->GetCameraBoom();
	OwningCharacter->InterpolateCameraTransform(DeltaTime, cameraBoom, camera);
	if (!bDisablePostProcessingManagement)
	{
		OwningCharacter->InterpolateCameraPostProcessing(DeltaTime, camera);
	}
}

void UCharacterCameraComponent::NewKeyPosition(FString IdentifierKey)
{
	if (CameraPositionsKeyMap.Contains(IdentifierKey))
	{
		LastKeyPosition = CurrentKeyPosition;
		FCameraKeyPosition& position = CameraPositionsKeyMap[IdentifierKey];
		CurrentKeyPosition = position;
	}
	else
	{
		UE_LOG(LogCaedeCamera, Error, TEXT("%s was not found for characters camera controller component, please check that!"), *IdentifierKey);
	}
}

void UCharacterCameraComponent::BeginPlay()
{
	CameraPositionsKeyMap.Empty(CameraKeyPositions.Num());
	for (int32 i = 0; i < CameraKeyPositions.Num(); i++)
	{
		CameraPositionsKeyMap.Add(CameraKeyPositions[i].IdentifierKey, CameraKeyPositions[i]);
	}

	CameraPostProcessingKeyMap.Empty(CameraPostProcessingCollection.Num());
	for (int32 i = 0; i < CameraPostProcessingCollection.Num(); i++)
	{
		CameraPostProcessingKeyMap.Add(CameraPostProcessingCollection[i].IdentifierKey, CameraPostProcessingCollection[i]);
	}

	if (IsActive())
	{
		if (CameraPositionsKeyMap.Contains(StartKeyPosition))
		{
			FCameraKeyPosition& position = CameraPositionsKeyMap[StartKeyPosition];
			CurrentKeyPosition = position;
			LastKeyPosition = CurrentKeyPosition;
		}
		else
		{
			UE_LOG(LogCaedeCamera, Error, TEXT("StartKeyPosition was not found for characters camera controller component, please check that!"));
		}

		if (bUseCustomCameraPostProcessingAtBeginPlay && CameraPostProcessingKeyMap.Contains(CustomStartKeyPostProcessing))
		{
			FCameraPostProcessingSettings& postProcessingKey = CameraPostProcessingKeyMap[CustomStartKeyPostProcessing];
			CurrentKeyPostProcessing = postProcessingKey;
			LastKeyPostProcessing = CurrentKeyPostProcessing;
		}
		else
		{
			FCameraPostProcessingSettings defaultSettings;
			defaultSettings.IdentifierKey = "Default";
			defaultSettings.CameraPostProcessingSettings = OwningCharacter->GetCamera()->PostProcessSettings;
			defaultSettings.CameraPostProcessingInterpolationSpeed = -1;
			CameraPostProcessingKeyMap.Add(defaultSettings.IdentifierKey, defaultSettings);

			CurrentKeyPostProcessing = defaultSettings;
			LastKeyPostProcessing = CurrentKeyPostProcessing;
		}
	}
}

void UCharacterCameraComponent::NewKeyPostProcessSettings(FString IdentifierKey)
{
	if (CameraPostProcessingKeyMap.Contains(IdentifierKey))
	{
		LastKeyPostProcessing = CurrentKeyPostProcessing;
		FCameraPostProcessingSettings& postProcessingSettings = CameraPostProcessingKeyMap[IdentifierKey];
		CurrentKeyPostProcessing = postProcessingSettings;
	}
	else
	{
		UE_LOG(LogCaedeCamera, Error, TEXT("%s was not found for characters camera controller component, please check that!"), *IdentifierKey);
	}
}

