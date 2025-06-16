// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossPatrol.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UBTTask_BossPatrol : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_BossPatrol();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
