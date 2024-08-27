// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"

#include "NPC_AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true; //Enables the service
	NodeName = "BTService_ChangeSpeed";
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	if(auto* NPC_Controller = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		if(auto* NPC = Cast<ANPC>(NPC_Controller->GetPawn()))
		{
			NPC->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}
