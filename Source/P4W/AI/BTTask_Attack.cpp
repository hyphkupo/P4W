// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interface/P4WCharacterAIInterface.h"
#include "Monster/P4WBoss.h"
#include "Kismet/GameplayStatics.h"
#include "Character/P4WCharacterPlayer_PLD.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

UBTTask_Attack::UBTTask_Attack()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AutoAttackBossMontageRef(TEXT("/Game/Animation/AM_AutoAttackBoss.AM_AutoAttackBoss"));
	if (AutoAttackBossMontageRef.Object)
	{
		AutoAttackBossMontage = AutoAttackBossMontageRef.Object;
	}
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IP4WCharacterAIInterface* AIPawn = Cast<IP4WCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	AAIController* AICon = OwnerComp.GetAIOwner();


	//FAICharacterAttackFinished OnAttackFinished;
	//OnAttackFinished.BindLambda(
	//	[&]()
	//	{
	//		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//	}
	//);

	//AIPawn->SetAIAttackDelegate(OnAttackFinished);
	//AIPawn->AttackByAI();

	AP4WBoss* BossPawn = Cast<AP4WBoss>(ControllingPawn);
	if (BossPawn)
	{
		UBlackboardComponent* BlackboardPtr = OwnerComp.GetBlackboardComponent();
		if (!BlackboardPtr)
		{
			return EBTNodeResult::Failed;
		}

		UObject* Target = BlackboardPtr->GetValueAsObject("Target");
		AActor* TargetPawn = Cast<AActor>(Target);

		//AP4WCharacterPlayer_PLD* PLDPawn = Cast<AP4WCharacterPlayer_PLD>(TargetPawn);
		//if (PLDPawn && PLDPawn->bIsSheltron)
		//{
		//	UGameplayStatics::ApplyDamage(TargetPawn, 5.0f * 0.85, AICon, ControllingPawn, nullptr);
		//}
		//else
		//{
		//	UGameplayStatics::ApplyDamage(TargetPawn, 5.0f, AICon, ControllingPawn, nullptr);
		//}

		//FTimerHandle AttackHandle;

		UAnimInstance* AnimInstance = BossPawn->GetMesh()->GetAnimInstance();
			//MulticastRPCAutoAttackBossAnimation();

		//AnimInstance->Montage_Play(AutoAttackBossMontage);
		//BossPawn->AutoAttack();

		//GetWorld()->GetTimerManager().SetTimer(
		//	AttackHandle,
		//	FTimerDelegate::CreateLambda([&]()
		//		{
		//			BossPawn->AutoAttack();
		//		}
		//	), 0.5f, false
		//);


		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}
