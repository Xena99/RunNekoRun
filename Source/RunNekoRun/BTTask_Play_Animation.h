// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Play_Animation.generated.h"

/**
 * 
 */
UCLASS()
class RUNNEKORUN_API UBTTask_Play_Animation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_Play_Animation(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
	void FinishExecute(UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type Result);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess=true))
	UAnimMontage* AnimMontage;
};
