

#pragma once

#include "NPCTask.h"
#include "CollectTask.generated.h"


UCLASS(BlueprintType, Blueprintable)
class ACollectTask : public ANPCTask
{
	GENERATED_UCLASS_BODY()

	/// The range the NPC will search for points to collect things
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float CollectionRange;

	//////////////////////////////////////////////////////////////////////////

	// Collection Types

	/// Is it a thief? :D
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	bool CollectItemsWithOwner;

	/// Is he looking for something?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	bool CollectOwnItems;

	/// Collect items for free or from gaja
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	bool CollectItemsWithoutOwner;

	//////////////////////////////////////////////////////////////////////////

	// After Collection Waiting

	/// Time to wait till continuing the task
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float WaitBetweenCollect;

	/// Random additional time to wait - between '0' and 'Random Wait Time'
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float RandomWaitTime;

	//////////////////////////////////////////////////////////////////////////

	// Finding an item in a place

	/// Max Number of attempts to find a item in a search place
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	int32 MaxPlaceSearchTries;

	/// Max Random additional number of attempts to find a item in a search place - between '0' and 'Max Random Place Search Tries'
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	int32 MaxRandomPlaceSearchTries;

	//////////////////////////////////////////////////////////////////////////

	// Finding a place to search and collect

	/// Probability between 0 .. 1 to select a place at random or take the nearest place
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	float SearchRandomAmount;

	//////////////////////////////////////////////////////////////////////////

	// What and where to collect

	/// Target points where to try to find and collect an item
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	TArray<class ANPCTargetPoint*> CollectionPlaces;

	/// Item class types to collect
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPC Task")
	TArray<class UClass*> ItemClassTypes;

};
