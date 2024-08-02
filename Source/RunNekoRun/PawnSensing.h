// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "PawnSensing.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNNEKORUN_API UPawnSensing : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPawnSensing();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Sensing radius
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
	float SensingRadius;

	// Field of view angle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
	float FieldOfView;

	// Detect pawns within the sensing radius and field of view
	void SensePawns();
};
