

#pragma once

#include "Engine/TargetPoint.h"
#include "Tasks/NPCTask.h"
#include "NPCTargetPoint.generated.h"

/**
 * 
 */
UCLASS(meta=(ChildCanTick))
class CAEDE_API ANPCTargetPoint : public ATargetPoint
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Target Point")
	TArray<TEnumAsByte<ENPCTaskType::Type>> AcceptableTasks;

	//////////////////////////////////////////////////////////////////////////

	// Patrol Point

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Patrol Point")
	float WaitDuration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Patrol Point")
	float RandomWaitDuratoin;

	//////////////////////////////////////////////////////////////////////////

	// Collection Point

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Collection Point")
	float CollectionZoneRadius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Collection Point")
	float FindItemRadius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Collection Point")
	float CollectionTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Collection Point")
	TArray<class UClass*> ItemTypes;

	//////////////////////////////////////////////////////////////////////////



};
