

#pragma once

#include "NPCTask.h"
#include "PatrolTask.generated.h"

UCLASS(BlueprintType, Blueprintable)
class APatrolTask : public ANPCTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	TArray<class ANPCTargetPoint*> PatrolPoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	bool RunningPatrol;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float WaitBetweenPatrol;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float RandomWaitTime;
};
