#include "SmartAIComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "SectorManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

USmartAIComponent::USmartAIComponent()
{

}

FGridNode* USmartAIComponent::FindPath(FVector StartPosition, FVector TargetPosition)
{
    // Create the start and target nodes
    FGridNode* StartNode = new FGridNode();
    StartNode->Position = StartPosition;

    FGridNode* TargetNode = new FGridNode();
    TargetNode->Position = TargetPosition;

    OpenList.Empty();
    ClosedList.Empty();

    OpenList.Add(StartNode);

    while (OpenList.Num() > 0)
    {
        FGridNode* CurrentNode = OpenList[0];
        UE_LOG(LogTemp, Log, TEXT("1. Processing Node at Position: %s with FCost: %f, HCost: %f"), *CurrentNode->Position.ToString(), CurrentNode->FCost(), CurrentNode->HCost);
        for (int i = 1; i < OpenList.Num(); i++)
        {
            if (OpenList[i]->FCost() < CurrentNode->FCost() || (OpenList[i]->FCost() == CurrentNode->FCost() && OpenList[i]->HCost < CurrentNode->HCost))
            {
                CurrentNode = OpenList[i];
            }
        }

        OpenList.Remove(CurrentNode);
        ClosedList.Add(CurrentNode);

        if (CurrentNode->Position.Equals(TargetNode->Position, 1.0f))
        {
            UE_LOG(LogTemp, Log, TEXT("Target reached at position: %s"), *CurrentNode->Position.ToString());
            TArray<FVector> Path;
            RetracePath(StartNode, CurrentNode, Path);
            return CurrentNode;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Target position not reached yet. Current: %s, Target: %s"), *CurrentNode->Position.ToString(), *TargetNode->Position.ToString());
        }

        for (FGridNode* Neighbor : GetNeighborNodes(CurrentNode))
        {
            UE_LOG(LogTemp, Log, TEXT("Getting Neighbors"));

            if (ClosedList.Contains(Neighbor))
                continue;

            float NewGCost = CurrentNode->GCost + FVector::Dist(CurrentNode->Position, Neighbor->Position);
            if (NewGCost < Neighbor->GCost || !OpenList.Contains(Neighbor))
            {
                Neighbor->GCost = NewGCost;
                Neighbor->HCost = CalculateHCost(Neighbor, TargetNode->Position);
                Neighbor->Parent = CurrentNode;

                if (!OpenList.Contains(Neighbor))
                {
                    OpenList.Add(Neighbor);
                }
            }
        }
    }
    return nullptr;
}

TArray<FGridNode*> USmartAIComponent::GetNeighborNodes(FGridNode* CurrentNode)
{
    TArray<FGridNode*> Neighbors;

    if (ASectorManager* SectorManager = ASectorManager::Instance)
    {
        TArray<AActor*> ActorsNear = SectorManager->GetActorsNear(GetOwner());

        for (AActor* Actor : ActorsNear)
        {
            if (Actor && Actor != GetOwner())
            {
                FGridNode* NeighborNode = new FGridNode();
                NeighborNode->Position = Actor->GetActorLocation();
                Neighbors.Add(NeighborNode);
            }
        }
    }

    return Neighbors;
}

float USmartAIComponent::CalculateHCost(FGridNode* Node, FVector TargetPosition)
{
    return FVector::Dist(Node->Position, TargetPosition);
}

void USmartAIComponent::RetracePath(FGridNode* StartNode, FGridNode* EndNode, TArray<FVector>& OutPath)
{
    TArray<FGridNode*> Path;
    FGridNode* CurrentNode = EndNode;

    while (CurrentNode != StartNode)
    {
        Path.Add(CurrentNode);
        CurrentNode = CurrentNode->Parent;
    }
    Path.Add(StartNode);

    // Manually reverse the array
    for (int32 i = Path.Num() - 1; i >= 0; i--)
    {
        OutPath.Add(Path[i]->Position);
    }
}

void USmartAIComponent::MoveNpcToPlayer(ACharacter* NPC, float DeltaTime)
{
    if (GetWorld() && NPC)
    {
        // Get the player's location
        ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (!PlayerCharacter)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player Actor is NULL"));
            return;
        }
        FVector PlayerLocation = PlayerCharacter->GetActorLocation();

        // Get the NPC's current location
        FVector NPCLocation = NPC->GetActorLocation();

        // Find the path
        FGridNode* PathNode = FindPath(NPCLocation, PlayerLocation);

        // Move the NPC along the path
        if (PathNode)
        {
            UE_LOG(LogTemp, Warning, TEXT("Moving Along the path."));
            TArray<FVector> Path;
            RetracePath(PathNode->Parent, PathNode, Path);

            if (Path.Num() > 0)
            {
                FVector NextPosition = Path[0];
                // Move NPC towards the next position
                FVector Direction = (NextPosition - NPCLocation).GetSafeNormal();
                float MovementSpeed = 300.0f; // Example movement speed
                NPC->SetActorLocation(NPCLocation + Direction * DeltaTime * MovementSpeed);
                UE_LOG(LogTemp, Warning, TEXT("NPC Location: %s"), *NPC->GetActorLocation().ToString());
            }
        }
        else
        {
           // UE_LOG(LogTemp, Warning, TEXT("PathNode is NULL."));

        }
    }
    else if (!GetWorld())
    {
        UE_LOG(LogTemp, Warning, TEXT("GetWorld is NULL"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("NPC is NULL"));
    }
}