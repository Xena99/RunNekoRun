// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation_BTTask.h"
#include "NPC_AIController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UFindPlayerLocation_BTTask::UFindPlayerLocation_BTTask(FObjectInitializer const& ObjectInitializer)
{
}

EBTNodeResult::Type UFindPlayerLocation_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const NPC_Controller = OwnerComp.GetAIOwner())
	{
		if (auto* const NPC_Pawn = NPC_Controller->GetPawn())
		{
			FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
			FNavLocation SearchLocation;
			if (SearchRandom)
			{
				if (auto* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
				{
					if (NavigationSystem->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, SearchLocation))
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), SearchLocation.Location);
						//Finish Task
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
				//Finish Task
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
