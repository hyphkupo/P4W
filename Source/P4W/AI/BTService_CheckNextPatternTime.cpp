// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckNextPatternTime.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_CheckNextPatternTime::UBTService_CheckNextPatternTime()
{
    NodeName = "Check Pattern Timer";
    bNotifyBecomeRelevant = true;
    Interval = 0.5f;
    bNotifyTick = true;
}

void UBTService_CheckNextPatternTime::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent* BlackboardPtr = OwnerComp.GetBlackboardComponent();
    if (!BlackboardPtr)
    {
        return;
    }

    bool bIsPatternPlaying = BlackboardPtr->GetValueAsBool("IsPatternPlaying");
    float NextPatternTime = BlackboardPtr->GetValueAsFloat("NextPatternTime");

    UWorld* World = OwnerComp.GetWorld();
    if (!World)
    {
        return;
    }

    float CurrentTime = World->GetTimeSeconds();

    if (!bIsPatternPlaying && CurrentTime >= NextPatternTime)
    {
        BlackboardPtr->SetValueAsBool("IsPatternPlaying", true);

        int32 LastPatternIndex = BlackboardPtr->GetValueAsInt("PatternIndex");
        //int32 NewPatternIndex;
        //do {
        //    NewPatternIndex = FMath::RandRange(0, 2);
        //} while (NewPatternIndex != LastPatternIndex); // 중복 방지

        LastPatternIndex += 1;
        if (LastPatternIndex > 2)
        {
            LastPatternIndex = 0;
        }
        BlackboardPtr->SetValueAsInt("PatternIndex", LastPatternIndex);
    }
}
