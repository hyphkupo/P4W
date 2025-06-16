// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/P4WBoss.h"
#include "AI/P4WAIController.h"
#include "CharacterStat/P4WCharacterStatComponent.h"
#include "P4W.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

AP4WBoss::AP4WBoss()
{
	AIControllerClass = AP4WAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BossMesh(TEXT("/Game/SciFi_Dog/Mesh/SK_SciFi_Dog_Skin4.SK_SciFi_Dog_Skin4"));
	if (BossMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(BossMesh.Object);
	}

	// Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Animation/ABP_P4WBoss.ABP_P4WBoss_C"));
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AutoAttackBossMontageRef(TEXT("/Game/Animation/AM_AutoAttackBoss.AM_AutoAttackBoss"));
	if (AutoAttackBossMontageRef.Object)
	{
		AutoAttackBossMontage = AutoAttackBossMontageRef.Object;
	}
}

void AP4WBoss::BeginPlay()
{
	Super::BeginPlay();
}

// Á¤Âû ¹üÀ§
float AP4WBoss::GetAIPatrolRadius()
{
	return 3000.0f;
}

float AP4WBoss::GetAIDetectRange()
{
	return 2000.0f;
}

float AP4WBoss::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float AP4WBoss::GetAITurnSpeed()
{
	return 2.0f;
}

void AP4WBoss::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AP4WBoss::AttackByAI()
{
	ProcessComboCommand();
}

void AP4WBoss::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}

void AP4WBoss::AutoAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AutoAttackBossMontage);
}

//void AP4WBoss::SetBossMaxEnmity(float Enmity)
//{
//	MaxEnmity = Enmity;
//}
