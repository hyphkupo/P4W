// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckNextPatternTime.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UBTService_CheckNextPatternTime : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckNextPatternTime();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
