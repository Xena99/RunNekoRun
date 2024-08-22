// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_AIController.h"

ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInitilizer)
{

}

void ANPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
		
	if (AEnemy_NPC* EnemyNPC = Cast<AEnemy_NPC>(InPawn))
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
