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
	if(auto const* NPC_Controller = Cast<ANPC_AIController>(Comp.GetAIOwner()))
	{
		auto const PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),
										0);
		FVector const PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
		if(UNavigationSystemV1* NavMesh = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerController,PlayerLocation);
			//Finish
			FinishLatentTask(Comp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
