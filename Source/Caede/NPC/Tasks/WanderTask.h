

#pragma once

#include "NPCTask.h"
#include "WanderTask.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AWanderTask : public ANPCTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float WanderRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	bool EnterBuildings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float WaitBetweenWander;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float RandomWaitTime;
};
