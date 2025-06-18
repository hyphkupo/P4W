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
    
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector TargetActorKey;

    // Å½»ö ¹Ý°æ
    UPROPERTY(EditAnywhere, Category = "Search")
    float SearchRadius = 2000.0f;

    UPROPERTY(EditAnywhere, Category = "Search")
    TSubclassOf<APawn> TargetClass;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float Damage = 30.0f;

    float PrevSpeed = 0.f;
    float ChargeSpeed = 3000.0f;
    
    float ChargeTime = 1.0f;

    UFUNCTION()
    void ChargeComplete(ACharacter* Character, UCharacterMovementComponent* MMoveComp, float StoredPrevSpeed, AActor* DamagedActor);
    
    APawn* AIPawn;
    AActor* ChosenActor;
    AAIController* AICon;
    ACharacter* AIChar;
    UCharacterMovementComponent* MoveComp;


};
