

#include "Caede.h"
#include "IdleTask.h"


AIdleTask::AIdleTask(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	TaskType = ENPCTaskType::Idle;
	WanderRange = 300;
	WanderWhileIdle = false;
	TurnWhileIdle = true;
	RandomWaitTime = 2;
	WaitBetweenWander = 2;
}


