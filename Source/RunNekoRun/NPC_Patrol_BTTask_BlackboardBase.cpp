// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Patrol_BTTask_BlackboardBase.h"
#include "NPC_AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy_NPC.h"

UNPC_Patrol_BTTask_BlackboardBase::UNPC_Patrol_BTTask_BlackboardBase(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Random Patrolling Task";
}

EBTNodeResult::Type UNPC_Patrol_BTTask_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get AI controller and NPC
	if (auto* const NPC_Controller = OwnerComp.GetAIOwner())
	{
		if (auto* const NPC_Pawn = NPC_Controller->GetPawn())
		{
			FVector NPCLocation = NPC_Pawn->GetActorLocation();
			FNavLocation RandomLocation;

			//Get the active Navigation Mesh
			if (UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				if (NavigationSystem->GetRandomPointInNavigableRadius(NPCLocation, SearchRadius, RandomLocation))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocation.Location);
					//UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *RandomLocation.Location.ToString());

					//Finish Task
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No Nav Mesh found"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Valid Pawn Found"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Valid AI Controller Found"));
	}
	return EBTNodeResult::Failed;
}
