

#include "Caede.h"
#include "NPCTargetPoint.h"


ANPCTargetPoint::ANPCTargetPoint(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	
	AcceptableTasks.Add(ENPCTaskType::Patrol);
	AcceptableTasks.Add(ENPCTaskType::Wander);
	AcceptableTasks.Add(ENPCTaskType::Collect);

	WaitDuration = 5;
	RandomWaitDuratoin = 5;

	CollectionZoneRadius = 750;
	CollectionTime = 5;
	FindItemRadius = 150;

	ItemTypes.Add(AItem::StaticClass());
}


