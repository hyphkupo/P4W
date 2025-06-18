// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToDist.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UBTTask_MoveToDist : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToDist();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	//UPROPERTY()
	//TObjectPtr<AActor> MoveTarget;

	
};
