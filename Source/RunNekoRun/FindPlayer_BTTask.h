// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindPlayer_BTTask.generated.h"

/**
 * 
 */
UCLASS()
class RUNNEKORUN_API UFindPlayer_BTTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UFindPlayer_BTTask(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, CAtegory = "Player Search", meta = (AllowPrivateAccess = "true"))
	float SearchRadius = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, CAtegory = "Random Search", meta = (AllowPrivateAccess = "true"))
	bool SearchRandom = false;
};
