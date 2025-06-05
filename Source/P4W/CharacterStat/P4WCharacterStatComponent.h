// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/P4WCharacterStat.h"
#include "P4WCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMpZeroDelegate);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/, float /*MaxHp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMpChangedDelegate, float /*CurrentMp*/, float /*MaxMp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P4W_API UP4WCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UP4WCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnMpZeroDelegate OnMpZero;

	FOnHpChangedDelegate OnHpChanged;
	FOnMpChangedDelegate OnMpChanged;

	void SetJobStat(int32 InNewJob);
	FORCEINLINE float GetCurrentJob() const { return CurrentJob; }
	FORCEINLINE void SetModifierStat(const FP4WCharacterStat& InModifierStat) { ModifierStat = InModifierStat; }
	FORCEINLINE FP4WCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	float ApplyDamage(float InDamage);
	
	float GetCurrentHp();
	float GetCurrentMp();
	float GetCurrentExp();

protected:
	//UPROPERTY()
	//float MaxHp;

	// �� �װ��� ������ ĳ���Ͱ� �ʱ�ȭ�� ������ �������� �ٲ� �� �ֱ� ������
	// �𸮾� ������ ������ ������ �ش� ������ ������� �ʵ��� Transient Ű���带 �߰�

	// �� �Լ��� ���� Hp ����
	void SetHp(float NewHp);
	void SetMp(float NewMp);
	void SetExp(float NewExp);

protected:
	// MaxHp, MaxMp: �⺻ ����

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentJob;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentMp;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentExp;

public:
	// ĳ������ �⺻ ����
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FP4WCharacterStat BaseStat;
	
	// ����, ���, ... � ���� �߰��Ǵ� ����
	// ���� �ʿ� ���� ��
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FP4WCharacterStat ModifierStat;
};
