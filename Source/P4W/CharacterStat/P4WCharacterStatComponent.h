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

	// 이 네가지 값들은 캐릭터가 초기화될 때마다 언제든지 바뀔 수 있기 때문에
	// 언리얼 엔진을 저장할 때에는 해당 정보가 저장되지 않도록 Transient 키워드를 추가

	// 이 함수를 통해 Hp 변경
	void SetHp(float NewHp);
	void SetMp(float NewMp);
	void SetExp(float NewExp);

protected:
	// MaxHp, MaxMp: 기본 스탯

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentJob;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentMp;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentExp;

public:
	// 캐릭터의 기본 스탯
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FP4WCharacterStat BaseStat;
	
	// 무기, 장비, ... 등에 의해 추가되는 스탯
	// 딱히 필요 없을 듯
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FP4WCharacterStat ModifierStat;
};
