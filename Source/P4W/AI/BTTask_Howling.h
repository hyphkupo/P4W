// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Howling.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UBTTask_Howling : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Howling();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    // 밀어내는 반경
    UPROPERTY(EditAnywhere, Category = "Push")
    float PushRadius = 1500.0f;

    // 힘의 세기 (위치 이동 거리)
    UPROPERTY(EditAnywhere, Category = "Push")
    float PushStrength = 1500.0f;

    // 보스 자신 포함 여부
    UPROPERTY(EditAnywhere, Category = "Push")
    bool bAffectSelf = false;

    // 밀리는 Pawn의 이동 시간
    UPROPERTY(EditAnywhere, Category = "Push")
    float LerpTime = 0.1f;

    // 이동에 걸리는 시간 (밀리는 속도)
    UPROPERTY(EditAnywhere, Category = "Push")
    float PushDuration = 1.5f;

    UPROPERTY(EditAnywhere, Category = "Push")
    UAnimMontage* PushMontage;

    UPROPERTY(EditAnywhere, Category = "Howling")
    TObjectPtr<class UAnimMontage> HowlingMontage;

    TMap<APawn*, FVector> InitialLocations;
    TMap<APawn*, FVector> TargetLocations;

    UPROPERTY()
    TObjectPtr<class ACharacter> AnimChar;

    //void UpdatePush(APawn* Pawn, FVector Start, FVector End, float Duration, float Elapsed);

protected:
    //void PlayPushAnimation();

    //UFUNCTION(Server, Unreliable)
    //void ServerRPCPushAnimation(ACharacter* Char);
        
};
