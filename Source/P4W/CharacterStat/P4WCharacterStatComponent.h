// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/P4WCharacterStat.h"
#include "P4WCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);


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
	//void SetJobStat(int32 InNewLevel);
	//FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }

	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	FORCEINLINE void SetModifierStat(const FP4WCharacterStat& InModifierStat) { ModifierStat = InModifierStat; }
	FORCEINLINE FP4WCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	float ApplyDamage(float InDamage);
	
	float GetCurrentHp();
	float GetMaxHp();

protected:
	//UPROPERTY()
	//float MaxHp;

	// �� �װ��� ������ ĳ���Ͱ� �ʱ�ȭ�� ������ �������� �ٲ� �� �ֱ� ������
	// �𸮾� ������ ������ ������ �ش� ������ ������� �ʵ��� Transient Ű���带 �߰�

	// �� �Լ��� ���� Hp ����
	void SetHp(float NewHp);

protected:
	UPROPERTY()
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentMp;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FP4WCharacterStat BaseStat;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FP4WCharacterStat ModifierStat;
};
