// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayer.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindPlayer::UBTTask_FindPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Player Location";
}

EBTNodeResult::Type UBTTask_FindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
