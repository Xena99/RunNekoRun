#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "SmartAIComponent.generated.h"

USTRUCT()
struct FGridNode
{
    GENERATED_BODY()

    FVector Position;
    float GCost;
    float HCost;
    float FCost() const { return GCost + HCost; }
    FGridNode* Parent;

    FGridNode()
        : Position(FVector::ZeroVector), GCost(0.f), HCost(0.f), Parent(nullptr) {}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RUNNEKORUN_API USmartAIComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USmartAIComponent();
    void MoveNpcToPlayer(ACharacter* NPC, float DeltaTime);

private:
    TArray<FGridNode*> OpenList;
    TArray<FGridNode*> ClosedList;
    FGridNode* FindPath(FVector StartPosition, FVector TargetPosition);
    void RetracePath(FGridNode* StartNode, FGridNode* EndNode, TArray<FVector>& OutPath);
    TArray<FGridNode*> GetNeighborNodes(FGridNode* CurrentNode);
    float CalculateHCost(FGridNode* Node, FVector TargetPosition);
};
