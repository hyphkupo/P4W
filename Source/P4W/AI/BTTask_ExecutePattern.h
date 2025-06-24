// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExecutePattern.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UBTTask_ExecutePattern : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ExecutePattern();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
