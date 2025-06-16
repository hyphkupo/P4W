// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_BossPatrol.h"

UBTTask_BossPatrol::UBTTask_BossPatrol()
{
}

EBTNodeResult::Type UBTTask_BossPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type();
}
