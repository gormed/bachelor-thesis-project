

#pragma once

#include "NPCTask.h"
#include "WorkTask.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AWorkTask : public ANPCTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	class ANPCTargetPoint* WorkPlace;

};
