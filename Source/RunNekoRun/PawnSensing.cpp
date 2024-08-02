// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnSensing.h"
#include "SectorManager.h"

// Sets default values for this component's properties
UPawnSensing::UPawnSensing()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SensingRadius = 1000.0f;  // default sensing radius
	FieldOfView = 90.0f;  // default field of view angle in degrees
}


// Called when the game starts
void UPawnSensing::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPawnSensing::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SensePawns();
}

void UPawnSensing::SensePawns()
{
    TArray<AActor*> NearbyActors = ASectorManager::Instance->GetActorsNear(GetOwner());
   // UE_LOG(LogTemp, Warning, TEXT("Sensing %d actors"), NearbyActors.Num());

    for (AActor* Actor : NearbyActors)
    {
        if (Actor != GetOwner())
        {
            APawn* Character = Cast<APawn>(Actor);
            if (Character)  // Check if the cast is successful
            {
                //UE_LOG(LogTemp, Warning, TEXT("Sensing character: %s"), *Character->GetName());

                FVector Direction = Character->GetActorLocation() - GetOwner()->GetActorLocation();
                float Distance = Direction.Size();

                if (Distance <= SensingRadius)
                {
                    Direction.Normalize();
                    float DotProduct = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Direction);
                    float CosAngle = FMath::Cos(FMath::DegreesToRadians(FieldOfView / 2.0f));

                    if (DotProduct > CosAngle)
                    {
                        //UE_LOG(LogTemp, Warning, TEXT("%s detected %s within field of view"), *GetOwner()->GetName(), *Character->GetName());
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Actor %s is not a character"), *Actor->GetName());
            }
        }
    }
}

