

#pragma once

#include "GameFramework/Actor.h"
#include "WorldTime.generated.h"

/**
 * 
 */
UCLASS()
class CAEDE_API AWorldTime : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time")
	int32 CurrentYears;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time")
	int32 CurrentMonths;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time")
	int32 CurrentDays;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time")
	int32 CurrentHours;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time")
	int32 CurrentMinutes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Time")
	float CurrentSeconds;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Time")
	float TimeScale;

	UFUNCTION(BlueprintCallable, Category = "Time", meta = (FriendlyName = "Get 2400 Format"))
	int32 Get2400Format();

	void Tick(float DeltaSecond) override;

private:

	static AWorldTime* Instance;
	float currentSeconds;
	
};
