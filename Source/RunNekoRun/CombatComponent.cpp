// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

UCombatComponent::UCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::Attack()
{
    if (GetOwner())
    {
        USkeletalMeshComponent* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

        if (Mesh && AttackMontages.Num() > 0)
        {
            UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
            if (AnimInstance)
            {
                bIsAttacking = true;
                UE_LOG(LogTemp, Warning, TEXT("Playing montage index: %d"), CombatMontageIndex);

                FOnMontageEnded MontageEndedDelegate;
                MontageEndedDelegate.BindUObject(this, &UCombatComponent::OnMontageEnded);
                AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontages[CombatMontageIndex]);

                AnimInstance->Montage_Play(AttackMontages[CombatMontageIndex]);
                UE_LOG(LogTemp, Warning, TEXT("Playing montage index: %s"), 
                    *AttackMontages[CombatMontageIndex]->GetName());
                CombatMontageIndex = (CombatMontageIndex + 1) % AttackMontages.Num();
            }
        }
    }
}

void UCombatComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (CombatMontageIndex >= AttackMontages.Num() || bInterrupted)
    {
        // Only set bIsAttacking to false if we've played all montages or if interrupted
        bIsAttacking = false;
        CombatMontageIndex = 0;  // Reset the index
    }
    else
    {
        // If there are more montages to play, call Attack again
        Attack();
    }
}