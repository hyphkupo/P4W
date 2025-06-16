// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "P4WGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UP4WGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// 전역 변수
	UPROPERTY(BlueprintReadWrite)
	float MaxEnmity = 0.0f;
	
};
