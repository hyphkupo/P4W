// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RandomAoE.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Gimmick/AOEField.h"
#include "NavigationSystem.h"

UBTTask_RandomAoE::UBTTask_RandomAoE()
{
	NodeName = TEXT("Spawn Random AOE");

    static ConstructorHelpers::FClassFinder<AActor> BP_AOERef(TEXT("/Game/Blueprint/BP_AOEField.BP_AOEField_C"));
    if (BP_AOERef.Succeeded())
    {
        BP_AOE = BP_AOERef.Class;
    }
}

EBTNodeResult::Type UBTTask_RandomAoE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AICon = OwnerComp.GetAIOwner();

    APawn* BossPawn = AICon ? AICon->GetPawn() : nullptr;
    if (!BossPawn)
    {
        return EBTNodeResult::Failed;
    }

    UWorld* World = BossPawn->GetWorld();
    if (!World)
    {
        return EBTNodeResult::Failed;
    }

    FVector Origin = FVector::ZeroVector;

    //if (bSpawnNearPlayer)
    //{
    //    // 플레이어 찾기
    //    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
    //    if (!PlayerPawn)
    //        return EBTNodeResult::Failed;

    //    Origin = PlayerPawn->GetActorLocation();
    //}
    //else
    //{
    //    // 보스 위치 사용
    //    if (AAIController* AICon = OwnerComp.GetAIOwner())
    //    {
    //        if (APawn* AIPawn = AICon->GetPawn())
    //            Origin = AIPawn->GetActorLocation();
    //    }
    //}

    for (int i = 0; i < 5; ++i)
    {
        UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
        if (NavSys)
        {
            FNavLocation RandomPoint;
            if (NavSys->GetRandomReachablePointInRadius(BossPawn->GetActorLocation(), 3000.0f, RandomPoint))
            {
                //FVector OutResult = RandomPoint.Location;
                SpawnPointArray.Add(RandomPoint.Location);
            }
        }
    }

    // 랜덤 위치 생성
    //FVector RandomOffset = FMath::VRand().GetSafeNormal2D() * FMath::FRandRange(0.f, SpawnRadius);
    //FVector SpawnLocation = Origin + RandomOffset;
    //SpawnLocation.Z += 10.0f;

    for (FVector Point : SpawnPointArray)
    {
        FActorSpawnParameters SpawnParams;
        //Point.Z = 30.0f;
        //AOEClass = AAOEField::StaticClass();
        World->SpawnActor<AAOEField>(BP_AOE, Point, FRotator::ZeroRotator, SpawnParams);
    }
    SpawnPointArray.Empty();

    return EBTNodeResult::Succeeded;
}
