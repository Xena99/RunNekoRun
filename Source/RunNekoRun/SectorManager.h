#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SectorManager.generated.h"

USTRUCT(BlueprintType)
struct FSector
{
    GENERATED_BODY()

    TArray<AActor*> ActorsInSector;
    FSector() {}
};

UCLASS()
class RUNNEKORUN_API ASectorManager : public AActor
{
    GENERATED_BODY()

public:
    // Size of each sector (e.g., 1000x1000 units)
    static constexpr float SectorSize = 1000.0f;

    // Map of sectors based on their grid position
    TMap<FIntPoint, FSector> Sectors;

    // Singleton instance for easy access
    static ASectorManager* Instance;

    ASectorManager();

    // Get the sector key based on location
    FIntPoint GetSectorKey(FVector Location) const;

    // Register an actor to a sector
    void RegisterActor(AActor* Actor);

    // Unregister an actor from a sector
    void UnregisterActor(AActor* Actor);

    // Find actors in and around a sector
    TArray<AActor*> GetActorsNear(AActor* Actor);

    void DestroyInstance();

};
