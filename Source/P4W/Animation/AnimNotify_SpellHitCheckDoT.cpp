// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SpellHitCheckDoT.h"
#include "Interface/P4WAnimationAttackInterface.h"

void UAnimNotify_SpellHitCheckDoT::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IP4WAnimationAttackInterface* AttackPawn = Cast<IP4WAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->SpellHitCheckDoT();
		}
	}
}
