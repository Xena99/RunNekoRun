// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_NPC.h"

// Sets default values
AEnemy_NPC::AEnemy_NPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy_NPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_NPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy_NPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* AEnemy_NPC::GetBehaviorTree() const
{
	return Tree;
}


