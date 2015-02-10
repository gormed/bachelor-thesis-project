

#include "Caede.h"

#include "NPC/CaedeAIController.h"
#include "NPC/CaedeNPC.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Navigation/CrowdManager.h"


ACaedeAIController::ACaedeAIController(const class FObjectInitializer& PCIP)
: Super(PCIP.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	BlackboardComponent = PCIP.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackBoard Component"));

	BehaviorComponent = PCIP.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("Behavior Component"));
	
	UCrowdFollowingComponent* CrowdFollowingComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());
	if (CrowdFollowingComponent != NULL)
	{
		CrowdFollowingComponent->SetCrowdSimulation(true);
	}

	//bWantsPlayerState = true;
	//PrimaryActorTick.bCanEverTick = true;
}

void ACaedeAIController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	ACaedeNPC* Bot = Cast<ACaedeNPC>(InPawn);

	// start behavior
	if (Bot && Bot->BotBehavior)
	{
		BlackboardComponent->InitializeBlackboard(*Bot->BotBehavior->BlackboardAsset);
		BehaviorComponent->StartTree(*Bot->BotBehavior);
	}
}

void ACaedeAIController::BeginInactiveState()
{
	Super::BeginInactiveState();
}

void ACaedeAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateControlRotation(DeltaTime, false);
}

void ACaedeAIController::SetFocalPoint(FVector FP, uint8 InPriority/* =EAIFocusPriority::Gameplay */)
{
	CurrentFocusPoint = FP;
	AAIController::SetFocalPoint(FP, InPriority);
}


