// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FVector APatrolPath::GetPathPoint(const int Index)
{
	return PathPoints[Index];	
}

int APatrolPath::NumPathPoints()
{
	return PathPoints.Num();
}


