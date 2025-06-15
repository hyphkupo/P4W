// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "P4WAIController.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API AP4WAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AP4WAIController();
	
	void RunAI();
	void StopAI();

protected:
	// 어떤 컨트롤러가 폰에 빙의할 때 발생되는 이벤트 함수
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

};
