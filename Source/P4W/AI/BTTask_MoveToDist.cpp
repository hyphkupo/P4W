// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToDist.h"
#include "P4WAI.h"
#include "AIController.h"
#include "Interface/P4WCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/P4WCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Character/P4WCharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Monster/P4WBoss.h"

UBTTask_MoveToDist::UBTTask_MoveToDist()
{
}

EBTNodeResult::Type UBTTask_MoveToDist::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return EBTNodeResult::Failed;
	}

	IP4WCharacterAIInterface* AIPawn = Cast<IP4WCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}


	AActor* MoveTarget = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (MoveTarget)
	{
		float Dist = FVector::Dist(Center, MoveTarget->GetActorLocation());
		if (Dist <= 300.0f)
		{
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Succeeded;
}
