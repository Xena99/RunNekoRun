// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Play_Animation.h"

#include "NPC_AIController.h"

UBTTask_Play_Animation::UBTTask_Play_Animation(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = "Play Animation";
}

EBTNodeResult::Type UBTTask_Play_Animation::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	if(auto* const NPC_Controller = Cast<ANPC_AIController>(Comp.GetAIOwner()))
	{
		if(auto* const NPC = Cast<ANPC>(NPC_Controller->GetPawn()))
		{
			UE_LOG(LogTemp, Display, TEXT("Play Animation Started0"));
			if(UAnimInstance* AnimInstance = NPC->GetMesh()->GetAnimInstance())
			{
				UE_LOG(LogTemp, Display, TEXT("Play Animation Started1"));
				if(AnimInstance && AnimMontage)
				{
					UE_LOG(LogTemp, Display, TEXT("Play Animation Started2"));
					AnimInstance->Montage_Play(AnimMontage);
					float MontageDuration = AnimMontage->GetPlayLength();
					FTimerHandle TimerHandle;
					NPC_Controller->GetWorld()->GetTimerManager().SetTimerForNextTick([this, &Comp]() {
						FinishLatentTask(Comp, EBTNodeResult::Succeeded);
					});
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_Play_Animation::FinishExecute(UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type Result)
{
	FinishLatentTask(OwnerComp, Result);	
}
