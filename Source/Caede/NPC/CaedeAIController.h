

#pragma once

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "AIController.h"

#include "Navigation/CrowdFollowingComponent.h"
#include "Navigation/CrowdManager.h"

#include "CaedeAIController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CAEDE_API ACaedeAIController : public AAIController
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Behaviour")
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Behaviour")
	class UBehaviorTreeComponent* BehaviorComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Behaviour")
	FVector CurrentFocusPoint;

	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////

	virtual void SetFocalPoint(FVector FP, uint8 InPriority/* =EAIFocusPriority::Gameplay */) override;

	virtual void Possess(class APawn* InPawn) override;

	virtual void BeginInactiveState() override;

	virtual void Tick(float DeltaTime) override;

	/* If there is line of sight to current enemy, start firing at them */
	//UFUNCTION(BlueprintCallable, Category = Behavior)
	//void ChaseTarget();
	
};
