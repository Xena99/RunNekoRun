// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PatrolPath.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NPC.generated.h"

UCLASS()
class RUNNEKORUN_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const;
	APatrolPath* GetPatrolPath() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

private:
	//Allowing BP derived from Enemy_NPC to access it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, CAtegory = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Patrol", meta = (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;
};
