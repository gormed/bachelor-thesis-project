

#include "Caede.h"
#include "NPC/DaycycleComponent.h"
#include "NPC/Tasks/NPCTask.h"
#include "WorldTime.h"


UDaycycleComponent::UDaycycleComponent(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	TickDaycycle = true;
	CurrentTask = NULL;
}

bool UDaycycleComponent::ForceBeginTask(class ANPCTask* Task, bool IngnoreCanBeCanceledByPlayer, bool IgnoreCanBeCanceledByOtherNPC, bool IgnoreWontEndNormally, bool IgnoreIsExtraordinaryTask)
{
	//if (AWorldTime::Get() != NULL 
	//	&& CurrentTask->CheckEndTask(AWorldTime::Get()->Get2400Format(),
	//	IgnoreWontEndNormally,
	//	IngnoreCanBeCanceledByPlayer,
	//	IgnoreCanBeCanceledByOtherNPC,
	//	IgnoreIsExtraordinaryTask))
	//{
	//	CurrentTask = Task;
	//	return true;
	//}
	return false;
}

bool UDaycycleComponent::ForceEndTask(class ANPCTask* NextTask, bool FindNextTask, bool IngnoreCanBeCanceledByPlayer, bool IgnoreCanBeCanceledByOtherNPC, bool IgnoreWontEndNormally, bool IgnoreIsExtraordinaryTask)
{
	//if (AWorldTime::Get() != NULL 
	//	&& CurrentTask->CheckEndTask(AWorldTime::Get()->Get2400Format(),
	//	IgnoreWontEndNormally, 
	//	IngnoreCanBeCanceledByPlayer, 
	//	IgnoreCanBeCanceledByOtherNPC, 
	//	IgnoreIsExtraordinaryTask))
	//{
	//	if (NextTask != NULL && !FindNextTask)
	//	{
	//		CurrentTask = NextTask;
	//		return true;
	//	}
	//	else
	//	{
	//		for (int32 i = 0; i < NPCTasks.Num(); i++)
	//		{
	//			if (NPCTasks[i] != NULL && NPCTasks[i]->CheckBeginTask(AWorldTime::Get()->Get2400Format(), false))
	//			{
	//				CurrentTask = NPCTasks[i];
	//				return true;
	//			}
	//		}
	//	}
	//}
	return false;
}

void UDaycycleComponent::TickComponentDaycycle(float DeltaTime)
{
	if (TickDaycycle && WorldTime != NULL && NPCTasks.Num() > 0)
	{
		if (NPCTasks.Num() > 0 && NPCTasks[0] == NULL)
		{
			return;
		}
		
		if (CurrentTask != NULL && CurrentTask->NextTask != NULL)
		{
			if (CurrentTask->CheckEndTask(
				WorldTime->Get2400Format(),
				false, false, false, false)
				&& CurrentTask != CurrentTask->NextTask)
			{
				ANPCTask* LastTask = CurrentTask;
				CurrentTask = CurrentTask->NextTask->CheckBeginTask(WorldTime->Get2400Format(), false);
				if (CurrentTask != LastTask)
				{
					OnTaskChanged.Broadcast(LastTask, CurrentTask);
				}
				return;
			}
		}
		else
		{
			UE_LOG(LogCaede, Error, TEXT("Failed to switch tasks, make sure all tasks have a next task and the NPC has it attached!"));
		}
			//for (int32 i = 0; i < NPCTasks.Num(); i++)
			//{
			//	if ((CurrentTask == NULL 
			//		&& 
			//		(NPCTasks[i] != NULL
			//		&& NPCTasks[i]->CheckBeginTask(AWorldTime::Get()->Get2400Format(), false)))
			//		|| 
			//		(/*CurrentTask != NULL && CurrentTask->CheckEndTask(AWorldTime::Get()->Get2400Format(), false, false, false, false)
			//		&&*/
			//		(NPCTasks[i] != NULL
			//		&& CurrentTask != NPCTasks[i]
			//		&& NPCTasks[i]->CheckBeginTask(AWorldTime::Get()->Get2400Format(), false))))
			//	{
			//		CurrentTask = NPCTasks[i];
			//		break;
			//	}
			//}
	}
}

void UDaycycleComponent::BeginPlay()
{
	if (WorldTime != NULL && NPCTasks.Num() > 0)
	{
		NPCTasks.Sort();
		// if the char is spawned take the first task
		if (CurrentTask == NULL && NPCTasks.Num() > 0 && NPCTasks[0] != NULL)
		{
			int32 i = 0;
			do 
			{
				if (NPCTasks[i] != NULL)
				{
					CurrentTask = NPCTasks[i]->CheckBeginTask(WorldTime->Get2400Format(), false);
				}
				i++;
			} while (CurrentTask == NULL && i < NPCTasks.Num());
			OnTaskChanged.Broadcast(NULL, CurrentTask);
		}
		//for (int32 i = 0; i < NPCTasks.Num(); i++)
		//{
		//	if (NPCTasks[i] != NULL
		//		&& NPCTasks[i]->CheckBeginTask(AWorldTime::Get()->Get2400Format(), false))
		//	{
		//		CurrentTask = NPCTasks[i];
		//		break;
		//	}
		//}
	}
}


