// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_IncrementPatrolIndex.generated.h"

/**
 * 
 */
UCLASS()
class RUNNEKORUN_API UBTTask_IncrementPatrolIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_IncrementPatrolIndex(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;

protected:
	enum class EDirection
	{
		Forward, Reverse
	};

	EDirection Direction = EDirection::Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bidirectional Patrolling")
	bool Bidirectional = true;
};
