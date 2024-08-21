// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NekoCharacter.generated.h"


UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class RUNNEKORUN_API UNekoCharacter : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNekoCharacter();


protected:

	UPROPERTY(EditAnywhere, Category = "NekoCharacter")
	float GroundDIstance = -100.0f;

	UPROPERTY(EditAnywhere, Category = "VaultingDistance")
	float VaultDistance = 200.0f;

	UPROPERTY(EditAnywhere, Category = "VaultHeightThreshold")
	float VaultHeightThreshold = 200.0f;

	UPROPERTY(EditAnywhere, Category = "VaultWidthThreshold")
	float VaultWidthThreshold = 200.0f;

	UPROPERTY(EditAnywhere, Category = "JumpMargin")
	float JumpMargin = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Vault Montage")
	UAnimMontage* VaultMontage;

	FVector WallThickness;

	FVector WallHeight;

	FVector WallHeight2;

	FVector WallNormal;

	FVector WallHitLocation;

	FVector WallForwardVector;

	FRotator WallRotator;

	ACharacter* CharacterOwner;

	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AlignToGround();

	UFUNCTION(BlueprintCallable, Category = "Vault")
	void VaultOrClimb();


	void ResetActorAfterVault();
	
	void ResetActorState();
};
