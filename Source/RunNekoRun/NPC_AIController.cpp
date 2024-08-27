// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_AIController.h"

#include "MyNekoCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "UObject/ConstructorHelpers.h"

ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInitilizer)
{
	SetUpAIPerceptionSystem();
}

void ANPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
		
	if (ANPC* EnemyNPC = Cast<ANPC>(InPawn))
	{
		if (UBehaviorTree* Tree = EnemyNPC->GetBehaviorTree())
		{
			//Run the behavior of the NPC by linking to the AI's Blackboard and manage the NPC's behavior based on the Behavior Tree
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(Tree->GetBlackboardAsset(), BlackboardComponent);
			Blackboard = BlackboardComponent;
			RunBehaviorTree(Tree);
		}
	}
}

void ANPC_AIController::SetUpAIPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if(SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));
		SightConfig->SightRadius = 500.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f; //keep detecting the player from the last seen loc
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		//Set Sight as the dominant sense
		GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPC_AIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		
	}
}

void ANPC_AIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if(auto* PlayerCharacter = Cast<AMyNekoCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}
