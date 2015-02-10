// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "Caede.h"

#include "NPC/CaedeNPC.h"
#include "NPC/DaycycleComponent.h"

#include "Components/BoxComponent.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"

#include "Character/EquipmentComponent.h"
#include "Character/InventoryComponent.h"

ACaedeNPC::ACaedeNPC(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	ViewAngle = 120.0f;
	ViewDistanceNear = 750.0f;
	ViewDistanceFar = 1200.0f;

	ListenRange = 1500.0f;
	SleepListenRange = 500.0f;

	SensingDistance = 250.0f;
	SleepSensingDistance = 100.0f;

	DaycycleComponent = PCIP.CreateDefaultSubobject<UDaycycleComponent>(this, TEXT("Daycycle Component"));

	IsInteractable = true;

	InternalNPCName = "Unnamed NPC";
	LocalizedNPCName = FText::FromString("Unnamed NPC");

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	//Components.Add(Inventory);
	//Health = PCIP.CreateDefaultSubobject<UHealthComponent>(this, TEXT("Health"));
	//Components.Add(Health);
}

void ACaedeNPC::BeginPlay()
{
	DaycycleComponent->OwningNPC = this;
	DaycycleComponent->BeginPlay();

	ACaedeCharacter::BeginPlay();
}

void ACaedeNPC::Tick(float DeltaSeconds)
{
	if (DaycycleComponent != NULL)
	{
		DaycycleComponent->TickComponentDaycycle(DeltaSeconds);
	}
	ACharacter::Tick(DeltaSeconds);
}