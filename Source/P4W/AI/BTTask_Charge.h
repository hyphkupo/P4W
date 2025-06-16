// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Charge.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UBTTask_Charge : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Charge();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    // 선택한 대상 액터
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector TargetActorKey;

    // 탐색할 반경
    UPROPERTY(EditAnywhere, Category = "Search")
    float SearchRadius = 1000.0f;

    // 찾을 액터의 클래스
    UPROPERTY(EditAnywhere, Category = "Search")
    TSubclassOf<APawn> TargetClass;

};
