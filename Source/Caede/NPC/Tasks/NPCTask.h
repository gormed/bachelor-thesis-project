

#pragma once

#include "GameFramework/Actor.h"
#include "NPCTask.generated.h"

/// The tasks a NPC can have
UENUM(BlueprintType)
namespace ENPCTaskType
{
	enum Type
	{
		Idle,
		Sleep,
		Wander,
		Patrol,
		Collect,
		Hobby,
		Work,
		Follow,
		Count,
		Invalid
	};
}

/**
 * A task that can be assigned to a placed NPC.
 */
UCLASS()
class CAEDE_API ANPCTask : public AActor
{
	GENERATED_UCLASS_BODY()

	/// Time to begin this task - in a 2400 format (http://military.onlineclock.net/chart/)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Base Task")
	int32 BeginTime;

	/// The task that comes before this
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "NPC Base Task")
	ANPCTask* BeforeTask;

	/// The task that comes after this
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "NPC Base Task")
	ANPCTask* NextTask;

	//////////////////////////////////////////////////////////////////////////

	/// Min Random Begin Offset
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Base Task")
	int32 MinBeginTimeOffset;
	
	/// Max Random Begin Offset
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Base Task")
	int32 MaxBeginTimeOffset;

	//////////////////////////////////////////////////////////////////////////

	/// The Type of that task for fast comparison
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "NPC Base Task")
	TEnumAsByte<ENPCTaskType::Type> TaskType;

	//////////////////////////////////////////////////////////////////////////

	/// Can task be canceled via interaction or aggression
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Base Task")
	bool CanBeCanceledByPlayer;

	/// Can a NPC cancel the task via interaction or aggression
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Base Task")
	bool CanBeCanceledByOtherNPC;

	/// Will the task continue forever until canceled manually
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Base Task")
	bool WontEndNormally;

	/// The task wont check for begin or end, it will take action if added to DaycycleComponent
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Base Task")
	bool IsExtraordinary;

	/// There will be special treatment for flying tasks
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	bool IsFlying;

	//////////////////////////////////////////////////////////////////////////

	ANPCTask* CheckBeginTask(int32 WorldTimeAs2400, bool IgnoreTime);

	bool CheckEndTask(int32 WorldTimeAs2400, bool EndUnnormal, bool EndedByPlayer, bool EndedByNPC, bool EndExtraordinary);

	void BeginPlay() override;

	//////////////////////////////////////////////////////////////////////////

	bool operator<(const ANPCTask& Other) const
	{
		return (BeginTime < Other.BeginTime);
	}
};
