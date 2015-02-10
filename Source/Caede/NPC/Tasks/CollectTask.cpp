

#include "Caede.h"
#include "CollectTask.h"


ACollectTask::ACollectTask(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	WaitBetweenCollect = 1;
	RandomWaitTime = 1;
	CollectionRange = 15000;

	CollectItemsWithoutOwner = true;
	CollectItemsWithOwner = false;
	CollectOwnItems = true;

	MaxRandomPlaceSearchTries = 3;
	MaxRandomPlaceSearchTries = 2;

	SearchRandomAmount = 0.5f;

	TaskType = ENPCTaskType::Collect;
}


