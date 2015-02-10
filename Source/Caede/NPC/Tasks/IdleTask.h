

#pragma once

#include "NPCTask.h"
#include "IdleTask.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AIdleTask : public ANPCTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	bool WanderWhileIdle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	bool TurnWhileIdle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float WanderRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float RandomWaitTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float WaitBetweenWander;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	class ANPCTargetPoint* IdlePlace;
};
