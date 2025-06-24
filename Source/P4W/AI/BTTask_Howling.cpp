// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Howling.h"
#include "AIController.h"
#include "Interface/P4WCharacterAIInterface.h"
#include "Monster/P4WBoss.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "Net/UnrealNetwork.h"
#include "Character/P4WCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Howling::UBTTask_Howling()
{
	NodeName = TEXT("Howling");

    static ConstructorHelpers::FObjectFinder<UAnimMontage> PushMontageRef(TEXT("/Game/Animation/AM_Push.AM_Push"));
    if (PushMontageRef.Object)
    {
        PushMontage = PushMontageRef.Object;
    }
    
    static ConstructorHelpers::FObjectFinder<UAnimMontage> HowlingMontageRef(TEXT("/Game/Animation/AM_Howling.AM_Howling"));
    if (HowlingMontageRef.Object)
    {
        HowlingMontage = HowlingMontageRef.Object;
    }
}

EBTNodeResult::Type UBTTask_Howling::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AICon = OwnerComp.GetAIOwner();
    
    APawn* BossPawn = AICon ? AICon->GetPawn() : nullptr;
    if (!BossPawn)
    {
        return EBTNodeResult::Failed;
    }
    //AP4WBoss* P4WBossChar = Cast<AP4WBoss>(BossPawn);
    //if (P4WBossChar)
    //{
    //    P4WBossChar->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    //}
 //       GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    // 

    UWorld* World = BossPawn->GetWorld();
    if (!World)
    {
        return EBTNodeResult::Failed;
    }

    FVector BossLocation = BossPawn->GetActorLocation();

    TArray<AActor*> AllPawns;
    UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), AllPawns);

    for (AActor* Actor : AllPawns)
    {
        APawn* TargetPawn = Cast<APawn>(Actor);
        
        if (!TargetPawn) continue;
        if (!bAffectSelf && TargetPawn == BossPawn) continue;

        float Distance = FVector::Dist(BossLocation, TargetPawn->GetActorLocation());
        if (Distance > PushRadius) continue;

        FVector Direction = (TargetPawn->GetActorLocation() - BossLocation).GetSafeNormal();
        FVector StartLoc = TargetPawn->GetActorLocation();
        FVector EndLoc = StartLoc + Direction * PushStrength;

        //if (ACharacter* Char = Cast<ACharacter>(TargetPawn))
        //{
        //    AnimChar = Char;
        //    Char->GetMesh()->GetAnimInstance()->Montage_Play(PushMontage);
        //    //MulticastRPCPushAnimation
        //}

        if (AP4WCharacterBase* Char = Cast<AP4WCharacterBase>(TargetPawn))
        {
            Char->MulticastRPCPushAnimation(PushMontage);
        }

        if (AP4WCharacterBase* BossChar = Cast<AP4WCharacterBase>(BossPawn))
        {
            BossChar->MulticastRPCHowlingAnimation(HowlingMontage);
        }

        float Interval = 0.02f;
        int32 Steps = PushDuration / Interval;

        for (int32 i = 1; i <= Steps; ++i)
        {
            float Alpha = (float)i / (float)Steps;

            FTimerHandle TimerHandle;
            World->GetTimerManager().SetTimer(TimerHandle, [TargetPawn, StartLoc, EndLoc, Alpha]()
                {
                    if (TargetPawn)
                    {
                        FVector NewLoc = FMath::Lerp(StartLoc, EndLoc, Alpha);
                        TargetPawn->SetActorLocation(NewLoc, true);
                    }
                }, Interval * i, false
            );
        }
    }

    UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();

    float DashDuration = 2.0f;
    //UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();

    //FTimerHandle TimerHandle;
    //World->GetTimerManager().SetTimer(
    //    TimerHandle,
    //    FTimerDelegate::CreateLambda([&]()
    //        {
    //            P4WBossChar->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    //        }
    //    ), 0.02f, false
    //);

    //BossChar->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);


    BossPawn->GetWorldTimerManager().SetTimerForNextTick([=]() {
        BC->SetValueAsBool("IsPatternPlaying", false);
        BC->SetValueAsFloat("NextPatternTime", BossPawn->GetWorld()->GetTimeSeconds() + FMath::FRandRange(5.0f, 10.0f));
        }
    );

    return EBTNodeResult::Succeeded;
}

//void UBTTask_Howling::ServerRPCPushAnimation_Implementation(ACharacter* Char)
//{
//    Char->GetMesh()->GetAnimInstance()->Montage_Play(PushMontage);
//    MulticastRPCPushAnimation(Char);
//}
//
//void UBTTask_Howling::MulticastRPCPushAnimation_Implementation(ACharacter* Char)
//{
//    Char->GetMesh()->GetAnimInstance()->Montage_Play(PushMontage);
//}
