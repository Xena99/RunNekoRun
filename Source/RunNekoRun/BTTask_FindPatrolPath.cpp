// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPath.h"

#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPath::UBTTask_FindPatrolPath(FObjectInitializer const& InFObjectInitializer)
	:UBTTask_BlackboardBase(InFObjectInitializer)

{
	NodeName = "Find Patrol Path";
}

EBTNodeResult::Type UBTTask_FindPatrolPath::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	if(auto* const NPC_Controller = Cast<ANPC_AIController>(Comp.GetAIOwner()))
	{
		//Blackboard component
		if(auto* BlackboardComp = NPC_Controller->GetBlackboardComponent ())
		{
			//Get Index and use it to get the Patrol Point
			int const Index = BlackboardComp->GetValueAsInt(GetSelectedBlackboardKey());
			if(auto const* NPC = Cast<ANPC>(NPC_Controller->GetPawn()))
			{
				if(APatrolPath* PatrolPath = NPC->GetPatrolPath())
				{
					FVector const PatrolPoint = PatrolPath->GetPathPoint(Index);

					//Transform the Patrol Point to world space
					FVector GlobalPoint = NPC->GetPatrolPath()->GetActorTransform().TransformPosition(PatrolPoint);
					BlackboardComp->SetValueAsVector(PatrolPointKey.SelectedKeyName, GlobalPoint);

					//Finish Task
					FinishLatentTask(Comp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
