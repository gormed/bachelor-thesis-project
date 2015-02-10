

#pragma once

#include "NPCTask.h"
#include "HobbyTask.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AHobbyTask : public ANPCTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	class ANPCTargetPoint* HobbyPlace;
};