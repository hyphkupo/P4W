// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Charge.h"
#include "AIController.h"
#include "Interface/P4WCharacterAIInterface.h"
#include "Monster/P4WBoss.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"

UBTTask_Charge::UBTTask_Charge()
{
    NodeName = TEXT("Charge");
}

EBTNodeResult::Type UBTTask_Charge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AICon = OwnerComp.GetAIOwner();
    APawn* AIPawn = AICon ? AICon->GetPawn() : nullptr;

    if (!AIPawn)
        return EBTNodeResult::Failed;

    UWorld* World = AIPawn->GetWorld();
    if (!World)
        return EBTNodeResult::Failed;

    FVector MyLocation = AIPawn->GetActorLocation();

    DrawDebugSphere(World, MyLocation, SearchRadius, 24, FColor::Green, false, 2.0f);

    TArray<AActor*> FoundActors;

    UGameplayStatics::GetAllActorsOfClass(World, TargetClass, FoundActors);

    TArray<AActor*> Candidates;
    for (AActor* Actor : FoundActors)
    {
        if (Actor != AIPawn && FVector::Dist(MyLocation, Actor->GetActorLocation()) <= SearchRadius)
        {
            Candidates.Add(Actor);

            DrawDebugSphere(World, Actor->GetActorLocation(), 50.0f, 12, FColor::Yellow, false, 2.0f);
        }
    }

    if (Candidates.Num() == 0)
    {
        return EBTNodeResult::Failed;
    }

    int32 Index = FMath::RandRange(0, Candidates.Num() - 1);
    AActor* ChosenActor = Candidates[Index];

    DrawDebugSphere(World, ChosenActor->GetActorLocation(), 80.0f, 16, FColor::Red, false, 2.0f);

    OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, ChosenActor);
    
    AICon->MoveToActor(ChosenActor);

    // Damage

    return EBTNodeResult::Succeeded;
}
