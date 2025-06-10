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

	// ��Ÿ�� ���� �̸� ���λ� (ComboAttack)
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	// �ִ� �޺� ���� (3��)
	UPROPERTY(EditAnywhere, Category = ComboData)
	uint8 MaxComboCount;

	// ������ ��� �ӵ�
	// �ִϸ��̼� �ּ��� �⺻ ��� �ӵ� ���� ������
	UPROPERTY(EditAnywhere, Category = ComboData)
	float FrameRate;

	// �Է��� ������ ���Դ����� Ȯ���ϴ� ������
	// ���� �ִϸ��̼� �ּ� ���� ����
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
};
