// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPlayer.generated.h"

/**
 * 
 */
UCLASS()
class RUNNEKORUN_API UBTTask_FindPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_FindPlayer(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, CAtegory = "Player Search", meta = (AllowPrivateAccess = "true"))
	float SearchRadius = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, CAtegory = "Random Search", meta = (AllowPrivateAccess = "true"))
	bool SearchRandom = false;
};
