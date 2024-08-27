// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"

#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = "ChasePlayer";
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	if(auto* NPC_Controller = Cast<ANPC_AIController>(Comp.GetAIOwner()))
	{
		if(auto const* BlackboardComponent = Cast<UBlackboardComponent>(NPC_Controller->GetBlackboardComponent()))
		{
			FVector const PlayerLocation = BlackboardComponent->GetValueAsVector(GetSelectedBlackboardKey());
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(NPC_Controller,PlayerLocation);
			//Finish
			FinishLatentTask(Comp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
