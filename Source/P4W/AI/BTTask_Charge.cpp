// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Charge.h"
#include "AIController.h"
#include "Interface/P4WCharacterAIInterface.h"
#include "Monster/P4WBoss.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/P4WCollision.h"
#include "Components/CapsuleComponent.h"
#include "Character/P4WCharacterBase.h"
#include "Character/P4WCharacterPlayer_PLD.h"

UBTTask_Charge::UBTTask_Charge()
{
    NodeName = TEXT("Charge");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Charge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    AICon = OwnerComp.GetAIOwner();
    AIPawn = AICon ? AICon->GetPawn() : nullptr;
    if (!AIPawn)
    {
        return EBTNodeResult::Failed;
    }

    UWorld* World = AIPawn->GetWorld();
    if (!World)
    {
        return EBTNodeResult::Failed;
    }

    AIChar = Cast<ACharacter>(AIPawn);
    MoveComp = AIChar->GetCharacterMovement();
    if (!AIChar || !MoveComp)
    {
        return EBTNodeResult::Failed;
    }

    FVector MyLocation = AIPawn->GetActorLocation();

    DrawDebugSphere(World, MyLocation, SearchRadius, 24, FColor::Green, false, 2.0f);

    TArray<AActor*> FoundActors;

    UGameplayStatics::GetAllActorsOfClass(World, TargetClass, FoundActors);

    TArray<AActor*> Actors;
    for (AActor* Actor : FoundActors)
    {
        if (Actor != AIPawn && FVector::Dist(MyLocation, Actor->GetActorLocation()) <= SearchRadius)
        {
            Actors.Add(Actor);

            DrawDebugSphere(World, Actor->GetActorLocation(), 50.0f, 12, FColor::Yellow, false, 2.0f);
        }
    }

    if (Actors.Num() == 0)
    {
        return EBTNodeResult::Failed;
    }

    int32 Index;
    Index = FMath::RandRange(0, Actors.Num() - 1);
    while (true)
    {
        AP4WCharacterBase* RanChar = Cast<AP4WCharacterBase>(Actors[Index]);
        if (RanChar->bIsDead)
        {
            continue;
        }
        else
        {
            break;
        }
    }
    
    ChosenActor = Actors[Index];

    DrawDebugSphere(World, ChosenActor->GetActorLocation(), 80.0f, 16, FColor::Red, false, 2.0f);

    OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, ChosenActor);
    
    AICon->MoveToActor(ChosenActor);

    PrevSpeed = MoveComp->MaxWalkSpeed;
    MoveComp->MaxWalkSpeed = ChargeSpeed;

    /*
    FTimerHandle SpeedHandle;
    ////GetWorld()->GetTimerManager().SetTimer(
    ////    SpeedHandle,
    ////    FTimerDelegate::CreateLambda([&]()
    ////        {
    ////            UCharacterMovementComponent* MoveComp = AIChar->GetCharacterMovement();
    ////            if (MoveComp)
    ////            {
    ////                MoveComp->MaxWalkSpeed = PrevSpeed;
    ////            }
    ////        }
    ////    ), 0.01f, false
    ////);
    //GetWorld()->GetTimerManager().SetTimer(SpeedHandle, [&]()
    //    {
    //        MoveComp->MaxWalkSpeed = PrevSpeed;
    //        UGameplayStatics::ApplyDamage(ChosenActor, Damage, nullptr, AIPawn, nullptr);
    //    }, 0.01f, false
    //);

    FTimerDelegate TimerDel;
    //TimerDel = FTimerDelegate::CreateUObject(this, &UBTTask_Charge::ChargeComplete, AIChar, MoveComp, PrevSpeed, ChosenActor);
    */


    
    // overlap되면 대미지 적용?
    //float Dist = FVector::Dist(AIPawn->GetActorLocation(), ChosenActor->GetActorLocation());
    ////ChargeTime = ChargeTime * (Dist / 1000.0f);
    //if (Dist <= 500.0f)
    //{
    //    MoveComp->MaxWalkSpeed = PrevSpeed;
    //    UGameplayStatics::ApplyDamage(ChosenActor, Damage, nullptr, AIChar, nullptr);
    //}
    //GetWorld()->GetTimerManager().SetTimer(SpeedHandle, TimerDel, ChargeTime, false);

    //float DetectRadius = 50.0f;

    


    //TArray<FOverlapResult> OverlapResults;
    //FCollisionQueryParams CollisionQueryParam;
    //bool bResult = World->OverlapMultiByChannel(
    //    OverlapResults,
    //    ChosenActor->GetActorLocation(),
    //    FQuat::Identity,
    //    CCHANNEL_P4WACTION,
    //    FCollisionShape::MakeSphere(DetectRadius),
    //    CollisionQueryParam
    //);

    //if (bResult)
    //{
    //    MoveComp->MaxWalkSpeed = PrevSpeed;
    //    UGameplayStatics::ApplyDamage(ChosenActor, Damage, nullptr, AIChar, nullptr);
    //}

    //UE_LOG(LogTemp, Log, TEXT("MaxEnmity: %f"), UP4WGameSingleton::Get().MaxEnmity);

    bool bIsTarget = false;
    
    return EBTNodeResult::InProgress;
    //return EBTNodeResult::Failed;

}

void UBTTask_Charge::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    float Dist = ChosenActor->GetDistanceTo(AIPawn);
    if (Dist <= 150.0f)
    {
        MoveComp->MaxWalkSpeed = PrevSpeed;

        AP4WCharacterPlayer_PLD* PLDPawn = Cast<AP4WCharacterPlayer_PLD>(ChosenActor);
        if (PLDPawn && PLDPawn->bIsSheltron)
        {
            UGameplayStatics::ApplyDamage(ChosenActor, Damage * 0.85, AICon, AIChar, nullptr);
        }
        else
        {
            UGameplayStatics::ApplyDamage(ChosenActor, Damage, AICon, AIChar, nullptr);
        }

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        //return EBTNodeResult::Succeeded;
    }
}

// 사용 x
void UBTTask_Charge::ChargeComplete(ACharacter* Character, UCharacterMovementComponent* MMoveComp, float StoredPrevSpeed, AActor* DamagedActor)
{
    MMoveComp->MaxWalkSpeed = StoredPrevSpeed;
    AP4WCharacterPlayer_PLD* PLDPawn = Cast<AP4WCharacterPlayer_PLD>(DamagedActor);
    if (PLDPawn && PLDPawn->bIsSheltron)
    {
        UGameplayStatics::ApplyDamage(DamagedActor, Damage * 0.85, nullptr, Character, nullptr);
    }
    else
    {
        UGameplayStatics::ApplyDamage(DamagedActor, Damage, nullptr, Character, nullptr);
    }
}
