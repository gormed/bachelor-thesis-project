

#pragma once

#include "NPCTask.h"
#include "FollowTask.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AFollowTask : public ANPCTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	class AActor* FollowActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	class ANPCTargetPoint* KnwonTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float FollowDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	bool CanLooseTarget;
};