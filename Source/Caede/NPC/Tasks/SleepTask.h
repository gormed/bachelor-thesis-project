

#pragma once

#include "NPCTask.h"
#include "SleepTask.generated.h"

UCLASS(BlueprintType, Blueprintable)
class ASleepTask : public ANPCTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	bool WakesUpDueAggression;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	class ANPCTargetPoint* SleepPlace;
};
