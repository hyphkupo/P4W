// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "P4WPLDComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UP4WPLDComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UP4WPLDComboActionData();

	// 몽타주 섹션 이름 접두사 (ComboAttack)
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	// 최대 콤보 개수 (3개)
	UPROPERTY(EditAnywhere, Category = ComboData)
	uint8 MaxComboCount;

	// 프레임 재생 속도
	// 애니메이션 애셋의 기본 재생 속도 값이 설정됨
	UPROPERTY(EditAnywhere, Category = ComboData)
	float FrameRate;

	// 입력이 사전에 들어왔는지를 확인하는 프레임
	// 공격 애니메이션 애셋 별로 지정
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
};
