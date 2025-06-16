// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RandomAoE.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Gimmick/AOEField.h"

UBTTask_RandomAoE::UBTTask_RandomAoE()
{
	NodeName = TEXT("Spawn Random AOE");
}

EBTNodeResult::Type UBTTask_RandomAoE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UWorld* World = GetWorld();
    if (!World || !AOEClass)
        return EBTNodeResult::Failed;

    FVector Origin = FVector::ZeroVector;

    if (bSpawnNearPlayer)
    {
        // 플레이어 찾기
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
        if (!PlayerPawn)
            return EBTNodeResult::Failed;

        Origin = PlayerPawn->GetActorLocation();
    }
    else
    {
        // 보스 위치 사용
        if (AAIController* AICon = OwnerComp.GetAIOwner())
        {
            if (APawn* AIPawn = AICon->GetPawn())
                Origin = AIPawn->GetActorLocation();
        }
    }

    // 랜덤 위치 생성
    FVector RandomOffset = FMath::VRand().GetSafeNormal2D() * FMath::FRandRange(0.f, SpawnRadius);
    FVector SpawnLocation = Origin + RandomOffset;
    SpawnLocation.Z += 10.0f;

    FActorSpawnParameters SpawnParams;
    World->SpawnActor<AAOEField>(AOEClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

    return EBTNodeResult::Succeeded;
}
