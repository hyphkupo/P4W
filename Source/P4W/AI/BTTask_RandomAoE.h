// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandomAoE.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UBTTask_RandomAoE : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_RandomAoE();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AOE")
	TSubclassOf<class AAOEField> BP_AOE;

	UPROPERTY(EditAnywhere, Category = "AOE")
	float SpawnRadius = 1200.0f;

	UPROPERTY(EditAnywhere, Category = "AOE")
	bool bSpawnNearPlayer = true;

	TArray<FVector> SpawnPointArray;
};
