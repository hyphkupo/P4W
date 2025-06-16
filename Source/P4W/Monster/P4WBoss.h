// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/P4WCharacterBase.h"
#include "Interface/P4WCharacterAIInterface.h"
#include "P4WBoss.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API AP4WBoss : public AP4WCharacterBase, public IP4WCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AP4WBoss();

protected:
	virtual void BeginPlay() override;

// AI Section
protected:
	// Á¤Âû ¹üÀ§
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;

public:
	void SetBossMaxEnmity(float Enmity);

	UPROPERTY()
	float MaxEnmity;
};
