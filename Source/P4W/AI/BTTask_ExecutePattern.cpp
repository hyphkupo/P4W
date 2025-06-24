// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ExecutePattern.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

UBTTask_ExecutePattern::UBTTask_ExecutePattern()
{
	NodeName = "Execute Pattern";
}

EBTNodeResult::Type UBTTask_ExecutePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
    int32 PatternIndex = BC->GetValueAsInt("PatternIndex");

    AAIController* AICon = OwnerComp.GetAIOwner();
    APawn* AIPawn = AICon ? AICon->GetPawn() : nullptr;

    if (!AIPawn) return EBTNodeResult::Failed;

    float Delay = 2.0f;     // 패턴 지속 시간
    FTimerHandle TimerHandle;
    AIPawn->GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=]() {
        BC->SetValueAsBool("IsPatternPlaying", false);
        BC->SetValueAsFloat("NextPatternTime", AIPawn->GetWorld()->GetTimeSeconds() + FMath::FRandRange(5.0f, 10.0f));
        }), Delay, false);

    return EBTNodeResult::Succeeded;
}
