

#include "Caede.h"
#include "WorldTime.h"


AWorldTime* AWorldTime::Instance = NULL;

AWorldTime::AWorldTime(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	CurrentHours = 8;
	currentSeconds = 0;
	TimeScale = 16;
}

void AWorldTime::Tick(float DeltaSecond)
{
	if (TimeScale < 0)
	{
		TimeScale = 0;
	}
	currentSeconds += (DeltaSecond * TimeScale);

	if (currentSeconds >= 59)
	{
		currentSeconds = 0;
		CurrentMinutes++;
	}
	CurrentSeconds = currentSeconds;
	if (CurrentMinutes > 59)
	{
		CurrentMinutes = 0;
		CurrentHours++;
	}
	if (CurrentHours > 23)
	{
		CurrentHours = 0;
		CurrentDays++;
	}
	if (CurrentDays > 30)
	{
		CurrentDays = 0;
		CurrentMonths++;
	}
	if (CurrentMonths > 12)
	{
		CurrentMonths = 0;
		CurrentYears++;
	}
	// :] okay that should be enough
	AActor::Tick(DeltaSecond);
}

int32 AWorldTime::Get2400Format()
{
	return (100 * CurrentHours) + CurrentMinutes;
}

//void AWorldTime::OnConstruction(const FTransform& Transform)
//{
//	if (Instance != NULL)
//	{
//		UE_LOG(LogCaede, Error, TEXT("There are more than one WorldTimes placed in the level, make sure there is only one!"));
//		return;
//	}
//	Instance = this;
//}

