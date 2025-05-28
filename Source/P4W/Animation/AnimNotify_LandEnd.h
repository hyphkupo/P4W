// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_LandEnd.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UAnimNotify_LandEnd : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;
	
};
