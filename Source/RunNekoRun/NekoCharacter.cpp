// Fill out your copyright notice in the Description page of Project Settings.


#include "NekoCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	CharacterOwner = Cast<ACharacter>(GetOwner());
	// ...
	
}


// Called every frame
void UNekoCharacter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	AlignToGround();

	//Vault();

}

void UNekoCharacter::AlignToGround()
{
	//Align the Character to the ground
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + FVector(0, 0, GroundDIstance);
	FCollisionQueryParams CollisionParams;
	FHitResult Hit;
	CollisionParams.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams))
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

void UNekoCharacter::VaultOrClimb()
{
	FVector Start = CharacterOwner->GetActorLocation();
	FVector End = Start + CharacterOwner->GetActorForwardVector() * VaultDistance;

	FCollisionObjectQueryParams ObjectsToTrace;
	ObjectsToTrace.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	FCollisionQueryParams ActorsToIgnore;
	ActorsToIgnore.AddIgnoredActor(CharacterOwner);

	//First Line Trace to detect the wall
	FHitResult Hit;

	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 5.0f);

	GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectsToTrace, ActorsToIgnore);
	if (Hit.Component == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Hit Component Found"));
		//no wall detected
		return;
	}

	WallNormal = Hit.Normal;
	WallHitLocation = Hit.Location;

	WallRotator = UKismetMathLibrary::MakeRotFromX(WallNormal);
	WallForwardVector = UKismetMathLibrary::GetForwardVector(WallRotator);

	//Get Wall Height
	Start = WallHitLocation + FVector(0, 0, VaultDistance) + WallForwardVector * -10.f;
	End = Start - FVector(0, 0, VaultDistance);

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5.0f);

	GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectsToTrace, ActorsToIgnore);
	WallHeight = Hit.Location;

	//Get Wall Thickness
	Start = WallHitLocation + FVector(0, 0, VaultDistance + 50.f) + WallForwardVector * -40.f;
	End = Start - FVector(0, 0, VaultDistance + 200.f);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f);

	GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectsToTrace, ActorsToIgnore);
	WallHeight2 = Hit.Location;

	WallThickness = WallHeight - WallHeight2;

	UE_LOG(LogTemp, Warning, TEXT("Heihgt: %f Thickness: %f"), WallHeight.Z, WallThickness.Z);

	if (fabs(WallHeight.Z) <= VaultHeightThreshold && fabs(WallThickness.Z) <= VaultWidthThreshold)
	{
		UE_LOG(LogTemp, Warning, TEXT("Vaulting"));

		//CharacterOwner->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CharacterOwner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

		FVector PositionInFrontOfWall = CharacterOwner->GetActorLocation() + WallForwardVector * 20;
		float TargetZ = WallHeight.Z + JumpMargin;

		FVector FinalPosition = FVector(PositionInFrontOfWall.X, PositionInFrontOfWall.Y, TargetZ);
		CharacterOwner->SetActorLocation(FinalPosition);

		//Play Vault Animation
		UAnimInstance* AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
		if (AnimInstance && VaultMontage)
		{
			UE_LOG(LogTemp, Warning, TEXT("Play Vaulting"));

			AnimInstance->Montage_Play(VaultMontage, 1.0f);

			float MontageDuration = VaultMontage->GetPlayLength();

			// Setup a timer to reset collision and rotation after the animation
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UNekoCharacter::ResetActorState, MontageDuration, false);
		}
	}
}

void UNekoCharacter::ResetActorState()
{
	CharacterOwner->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CharacterOwner->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	FRotator ActorRotator = WallRotator;
	ActorRotator.Yaw += 180;
	ActorRotator.Roll = CharacterOwner->GetActorRotation().Roll;
	ActorRotator.Pitch = CharacterOwner->GetActorRotation().Pitch;

	//CharacterOwner->SetActorRotation(ActorRotator);
}