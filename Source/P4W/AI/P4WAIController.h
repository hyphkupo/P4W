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
	// � ��Ʈ�ѷ��� ���� ������ �� �߻��Ǵ� �̺�Ʈ �Լ�
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

};
