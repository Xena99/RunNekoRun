// Fill out your copyright notice in the Description page of Project Settings.


#include "NekoCharacter.h"
#include "Landscape.h"

// Sets default values for this component's properties
UNekoCharacter::UNekoCharacter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNekoCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UNekoCharacter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + FVector(0, 0, MaxDistance);
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	if(GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams))
	{
		FVector GroundNormal = Hit.ImpactNormal;
		FVector ForwardVector = GetOwner()->GetActorForwardVector();

		FVector NewForwardVector = FVector::VectorPlaneProject(ForwardVector, GroundNormal).GetSafeNormal();
		FVector NewRightVector = FVector::CrossProduct(GroundNormal, NewForwardVector).GetSafeNormal();
		FVector NewUpVector = FVector::CrossProduct(NewForwardVector, NewRightVector);
		FMatrix NewRotationMatrix = FMatrix(NewForwardVector, NewRightVector, NewUpVector, FVector::ZeroVector);
		FRotator NewRotation = NewRotationMatrix.Rotator();
		GetOwner()->SetActorRotation(FRotator(NewRotation.Pitch, GetOwner()->GetActorRotation().Yaw, NewRotation.Roll));
	}
}

