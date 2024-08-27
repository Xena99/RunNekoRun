// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPatrolIndex.h"

#include "AIController.h"
#include "NPC.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_IncrementPatrolIndex::UBTTask_IncrementPatrolIndex(FObjectInitializer const& ObjectInitializer)
	:UBTTask_BlackboardBase(ObjectInitializer) //initializing the base class to access the blackboard properly 
{
	NodeName = "Increment Patrol Index";
}

EBTNodeResult::Type UBTTask_IncrementPatrolIndex::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	if(auto* const NPC_Controller = Cast<ANPC_AIController>(Comp.GetAIOwner()))
	{
		if(auto* BlackboardComp = NPC_Controller->GetBlackboardComponent())
		{
			if(auto* const NPC = Cast<ANPC>(NPC_Controller->GetPawn()))
			{
				if(Bidirectional)
				{
					int MinIndex = 0;
					int MaxIndex = NPC->GetPatrolPath()->NumPathPoints() - 1;
					int Index = BlackboardComp->GetValueAsInt(GetSelectedBlackboardKey());
					if(Index >= MaxIndex && Direction == EDirection::Forward)
					{
						Direction = EDirection::Reverse;
					}
					else if(Index == MinIndex && Direction == EDirection::Reverse)
					{
						Direction = EDirection::Forward;
					}

					BlackboardComp->SetValueAsInt(GetSelectedBlackboardKey(),
						(Direction  == EDirection::Forward ? ++Index : (--Index)%NPC->GetPatrolPath()->NumPathPoints()));
					UE_LOG(LogTemp, Warning, TEXT("Index: %d"), Index);
					//Finish
					FinishLatentTask(Comp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
