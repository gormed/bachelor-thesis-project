

#include "Caede.h"
#include "FollowTask.h"


AFollowTask::AFollowTask(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	TaskType = ENPCTaskType::Follow;
}


