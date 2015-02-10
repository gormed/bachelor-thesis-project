

#include "Caede.h"
#include "NPCTask.h"


ANPCTask::ANPCTask(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	BeginTime = 800;
	TaskType = ENPCTaskType::Invalid;

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

ANPCTask* ANPCTask::CheckBeginTask(int32 WorldTimeAs2400, bool IgnoreTime)
{
	if (IgnoreTime)
	{
		return this;
	}
	if (NextTask == NULL)
	{
		UE_LOG(LogCaede, Error, TEXT("%s has no next task set! If there is only one just make it point to itself"));
		return NULL;
	}
	int32 randomBegin = FMath::RandRange(MinBeginTimeOffset, MaxBeginTimeOffset);

	// if we have a task that lasts from the current to the next day
	if (BeginTime >= NextTask->BeginTime)
	{
		// begin if in the outgoing day
		if (WorldTimeAs2400 >= BeginTime)
		{
			return this;
		}
		// begin if in the beginning day
		if (WorldTimeAs2400 < NextTask->BeginTime)
		{
			return this;
		}
	}
	if (WorldTimeAs2400 >= BeginTime + randomBegin && WorldTimeAs2400 < NextTask->BeginTime)
	{
		return this;
	}
	return NextTask->CheckBeginTask(WorldTimeAs2400, IgnoreTime);
}

bool ANPCTask::CheckEndTask(int32 WorldTimeAs2400, bool EndUnnormal, bool EndedByPlayer, bool EndedByNPC, bool EndExtraordinary)
{
	//int32 randomEnd = FMath::RandRange(MinEndTimeOffset, MaxEndTimeOffset);
	if (WontEndNormally == EndUnnormal
		|| CanBeCanceledByPlayer == EndedByPlayer
		|| CanBeCanceledByOtherNPC == EndedByNPC
		|| IsExtraordinary == EndExtraordinary)
	{
		return true;
	}
	//if (WorldTimeAs2400 > EndTime + randomEnd)
	//{
	//	return true;
	//}
	return false;
}

void ANPCTask::BeginPlay()
{
	if (NextTask != NULL)
	{
		NextTask->BeforeTask = this;
	}
	else
	{
		UE_LOG(LogCaede, Error, TEXT("%s has no next task set! If there is only one just make it point to itself"));
	}
}


