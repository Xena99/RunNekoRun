// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemy_NPC.h"
#include "NPC_AIController.generated.h"

/**
 * 
 */
UCLASS()
class RUNNEKORUN_API ANPC_AIController : public AAIController
{
	GENERATED_BODY()

public:

	explicit ANPC_AIController(FObjectInitializer const& ObjectInitilizer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
