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
    // ������ ��� ����
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector TargetActorKey;

    // Ž���� �ݰ�
    UPROPERTY(EditAnywhere, Category = "Search")
    float SearchRadius = 1000.0f;

    // ã�� ������ Ŭ����
    UPROPERTY(EditAnywhere, Category = "Search")
    TSubclassOf<APawn> TargetClass;

};
