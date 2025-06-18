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
    // �о�� �ݰ�
    UPROPERTY(EditAnywhere, Category = "Push")
    float PushRadius = 1500.0f;

    // ���� ���� (��ġ �̵� �Ÿ�)
    UPROPERTY(EditAnywhere, Category = "Push")
    float PushStrength = 1500.0f;

    // ���� �ڽ� ���� ����
    UPROPERTY(EditAnywhere, Category = "Push")
    bool bAffectSelf = false;

    // �и��� Pawn�� �̵� �ð�
    UPROPERTY(EditAnywhere, Category = "Push")
    float LerpTime = 0.1f;

    // �̵��� �ɸ��� �ð� (�и��� �ӵ�)
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
