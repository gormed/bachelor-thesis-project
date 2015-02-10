

#include "Caede.h"
#include "PatrolTask.h"


APatrolTask::APatrolTask(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	WaitBetweenPatrol = 5;
	TaskType = ENPCTaskType::Patrol;
}


