// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayerLocation.h"

#include "NavigationSystem.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& InFObjectInitializer)
	:UBTTask_BlackboardBase(InFObjectInitializer)
{
	NodeName = "Find Player";
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	if(auto* const NPC_Controller = Cast<ANPC_AIController>(Comp.GetAIOwner()))
	{
		if(UBlackboardComponent* BlackboardComponent =  NPC_Controller->GetBlackboardComponent())
		{
			if(auto* const PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				FVector PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
				if(SearchRandom)
				{
					if(UNavigationSystemV1* const NavMesh = UNavigationSystemV1::GetCurrent(GetWorld()))
					{
						FNavLocation Location;
						if(NavMesh->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Location))
						{
							BlackboardComponent->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
						}
					}
				}
				else
				{
					BlackboardComponent->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
				}
				FinishLatentTask(Comp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
