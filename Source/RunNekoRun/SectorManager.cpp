#include "SectorManager.h"
#include "Engine/World.h"

ASectorManager* ASectorManager::Instance = nullptr;

ASectorManager::ASectorManager()
{
    PrimaryActorTick.bCanEverTick = false; // Disable ticking if not needed

    if (!Instance)
    {
        Instance = this;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Another instance of ASectorManager already exists!"));
    }
}

FIntPoint ASectorManager::GetSectorKey(FVector Location) const
{
    int32 X = FMath::FloorToInt(Location.X / SectorSize);
    int32 Y = FMath::FloorToInt(Location.Y / SectorSize);
    return FIntPoint(X, Y);
}

void ASectorManager::RegisterActor(AActor* Actor)
{
    if (!Actor || !Actor->IsValidLowLevel() || Actor->IsPendingKill() || IsValid(Actor)) {
        UE_LOG(LogTemp, Warning, TEXT("Actor is invalid, null, or being destroyed."));
        return;
    }

    FIntPoint SectorKey = GetSectorKey(Actor->GetActorLocation());
    FSector* Sector = Sectors.Find(SectorKey);
    if (!Sector) {
        Sector = &Sectors.Add(SectorKey, FSector());
    }
    if (Sector) {
        Sector->ActorsInSector.Add(Actor);
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Failed to find or create sector for key."));
    }
}

void ASectorManager::UnregisterActor(AActor* Actor)
{
    if (!Actor)
    {
        return;
    }

    FIntPoint SectorKey = GetSectorKey(Actor->GetActorLocation());
    if (FSector* Sector = Sectors.Find(SectorKey))
    {
        Sector->ActorsInSector.Remove(Actor);
        UE_LOG(LogTemp, Warning, TEXT("Actor %s unregistered from sector %d, %d"), *Actor->GetName(), SectorKey.X, SectorKey.Y);
    }
}

TArray<AActor*> ASectorManager::GetActorsNear(AActor* Actor)
{
    TArray<AActor*> Result;
    if (!Actor || Sectors.IsEmpty())
    {
        return Result;
    }

    FIntPoint SectorKey = GetSectorKey(Actor->GetActorLocation());

    // Get actors in the same sector and neighboring sectors
    for (int32 X = SectorKey.X - 1; X <= SectorKey.X + 1; ++X)
    {
        for (int32 Y = SectorKey.Y - 1; Y <= SectorKey.Y + 1; ++Y)
        {
            FIntPoint NeighborKey(X, Y);
            FSector* Sector = Sectors.Find(NeighborKey);
            if (Sector) // Check if the sector was found.
            {
                Result.Append(Sector->ActorsInSector);
            }
        }
    }

    return Result;
}

void ASectorManager::DestroyInstance()
{
    if (Instance)
    {
        Instance->ConditionalBeginDestroy();
        Instance = nullptr;
    }
}
