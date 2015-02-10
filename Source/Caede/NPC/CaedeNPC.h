// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BehaviorTree/BehaviorTree.h"
#include "CaedeNPC.generated.h"

//////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartInteractionSignature, ACaedeCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopInteractionSignature, ACaedeCharacter*, Character);

//////////////////////////////////////////////////////////////////////////

/**
 * The basical caede game pawn for any npc.
 */
UCLASS()
class CAEDE_API ACaedeNPC : public ACaedeCharacter
{
	GENERATED_UCLASS_BODY()

	//////////////////////////////////////////////////////////////////////////

	// AI

	/// The distance the NPC will sense another actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	float SensingDistance;

	/// The distance the NPC will sense another actor while asleep
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	float SleepSensingDistance;

	/// The angle the NPC can see
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	float ViewAngle;

	/// The range the NPC can see actors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	float ViewDistanceNear;

	/// The range the NPC can see only positions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	float ViewDistanceFar;

	/// The range the NPC can hear things
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	float ListenRange;

	/// The range the NPC can hear things while asleep
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	float SleepListenRange;

	//////////////////////////////////////////////////////////////////////////

	// NPC States -> overrides tasks

	/// True if the NPC is willing to fight (can be triggered through several ways)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	bool IsAggressive;

	/// True if the NPC has reached his sleep place and is sleeping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
	bool IsAsleep;

	//////////////////////////////////////////////////////////////////////////

	/// Daycycle Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Behaviour")
	class UDaycycleComponent* DaycycleComponent;

	/// AI Behaviour Tree
	UPROPERTY(EditAnywhere, Category = "Behaviour")
	class UBehaviorTree* BotBehavior;

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintAssignable, Category = "Pawn Interaction")
	FStartInteractionSignature OnStartInteraction;

	UPROPERTY(BlueprintAssignable, Category = "Pawn Interaction")
	FStopInteractionSignature OnStopInteraction;

	//////////////////////////////////////////////////////////////////////////

	/// NPC Name

	/// The internal and dev NPC name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FString InternalNPCName;

	/// The localized NPC name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Localized, Category = "Character")
	FText LocalizedNPCName;

	//////////////////////////////////////////////////////////////////////////

	/// Can the player interact with me?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Interaction")
	bool IsInteractable;

	//////////////////////////////////////////////////////////////////////////

	void Tick(float DeltaSeconds) override;

	void BeginPlay() override;
};
