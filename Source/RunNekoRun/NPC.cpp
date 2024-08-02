// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "SectorManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "SmartAIComponent.h"
#include "Engine/World.h"

// Sets default values
ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;

	if (GetMesh())
	{
		// Set a default position for the mesh (relative to the character's root)
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		// Find and assign the skeletal mesh
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> 
			MeshFinder(TEXT("SkeletalMesh'/Game/BookHeadMonster/Meshes/BookHeadMonster.BookHeadMonster'"));

		if (MeshFinder.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(MeshFinder.Object);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find Skeleton Mesh"));
		}

		// Find and assign the animation blueprint
		static ConstructorHelpers::FObjectFinder<UAnimInstance>
			AnimFinder(TEXT("AnimBlueprint'/Game/BookHeadMonster/Animations/BookHeadedMonster_AnimBP.BookHeadedMonster_AnimBP'"));
		if (AnimFinder.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(AnimFinder.Object.GetClass());
			UE_LOG(LogTemp, Warning, TEXT("Animation BP loaded successfully."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to find Animation BP"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MeshComponent is null"));
	}

	// Initialize custom AI component
	SmartAIComponent = CreateDefaultSubobject<USmartAIComponent>(TEXT("SmartAIComponent"));
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Adding Characters"));
	ASectorManager::Instance->RegisterActor(this);
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (HasLineOfSightToPlayer(Player))
	{
		// Move directly towards the player
		FVector TargetLocation = Player->GetActorLocation();
		FVector NPCLocation = GetActorLocation();

		// Ensure the NPC only rotates around the yaw axis
		TargetLocation.Z = NPCLocation.Z;

		FVector Direction = (TargetLocation - NPCLocation).GetSafeNormal();
		Direction.Normalize();

		SetActorLocation(GetActorLocation() + Direction * 40.0f * DeltaTime);

		FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		SetActorRotation(NewRotation);

		//UE_LOG(LogTemp, Warning, TEXT("Moving directly towards the player %s"), *this->GetActorLocation().ToString());
	}
	else
	{
		if (SmartAIComponent)
		{
			//UE_LOG(LogTemp, Warning, TEXT("NPC character: %s"), *this->GetName());
			SmartAIComponent->MoveNpcToPlayer(this, DeltaTime);
		}
	}
}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Check if the Sector Manager exists and unregister this actor
	if (ASectorManager::Instance)
	{
		ASectorManager::Instance->UnregisterActor(this);
	}
}

bool ANPC::HasLineOfSightToPlayer(AActor* Player)
{
	if (!Player || !GetWorld()) return false;

	FVector StartPosition = GetActorLocation();
	FVector EndPosition = Player->GetActorLocation();
	FHitResult HitResult;

	// Perform the line trace
	bool bHasLOS = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPosition,
		EndPosition,
		ECC_Visibility,  // Use appropriate collision channel
		FCollisionQueryParams(FName(TEXT("LOS")), true, this)
	);

	// If we hit something, check if it's not the player
	if (bHasLOS && HitResult.GetActor() != Player)
	{
		return false;  // There's something blocking the view
	}

	return true;  // Nothing blocking the view
}
