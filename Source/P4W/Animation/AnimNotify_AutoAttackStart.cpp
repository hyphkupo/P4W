// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AutoAttackStart.h"
#include "GameFramework/Controller.h"

void UAnimNotify_AutoAttackStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

}
