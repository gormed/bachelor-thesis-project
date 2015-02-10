

#pragma once

#include "Components/ActorComponent.h"
#include "DaycycleComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTaskChangedSignature, class ANPCTask*, LastTask, class ANPCTask*, NextTask);

/**
 * 
 */
UCLASS(BlueprintType)
class CAEDE_API UDaycycleComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Behaviour")
	TArray<class ANPCTask*> NPCTasks;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Behaviour")
	class ACaedeNPC* OwningNPC;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Behaviour")
	bool TickDaycycle;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Behaviour")
	class ANPCTask* CurrentTask;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Behaviour")
	class AWorldTime* WorldTime;

	//////////////////////////////////////////////////////////////////////////

	/// Forces a task to start and optionally ignore different properties of the current tasks to get aborted
	UFUNCTION(BlueprintCallable, Category = "NPC Task", meta = (FriendlyName = "Force Begin Task (Stub)"))
	bool ForceBeginTask(class ANPCTask* Task, 
		bool IngnoreCanBeCanceledByPlayer, bool IgnoreCanBeCanceledByOtherNPC, 
		bool IgnoreWontEndNormally, bool IgnoreIsExtraordinaryTask);

	/// Ends the current task - if Task is set and FindNextTask is false, this will be the next Task otherwise the next Task is checked against time
	UFUNCTION(BlueprintCallable, Category = "NPC Task", meta = (FriendlyName = "Force Begin Task (Stub)"))
	bool ForceEndTask(class ANPCTask* Task, bool FindNextTask,
		bool IngnoreCanBeCanceledByPlayer, bool IgnoreCanBeCanceledByOtherNPC,
		bool IgnoreWontEndNormally, bool IgnoreIsExtraordinaryTask);

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintAssignable, Category = "NPC Task")
	FOnTaskChangedSignature OnTaskChanged;

	//////////////////////////////////////////////////////////////////////////

	void TickComponentDaycycle(float DeltaTime);

	void BeginPlay();

};
