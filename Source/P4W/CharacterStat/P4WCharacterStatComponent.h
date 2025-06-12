// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/P4WCharacterStat.h"
#include "P4WCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMpZeroDelegate);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float /*CurrentHp*/, float /*MaxHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMpChangedDelegate, float /*CurrentMp*/, float /*MaxMp*/);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FP4WCharacterStat& /*BaseStat*/, const FP4WCharacterStat& /*ModifierStat*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P4W_API UP4WCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UP4WCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void ReadyForReplication() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetNewMaxHp(const FP4WCharacterStat& InBaseStat, const FP4WCharacterStat& InModifierStat);
	void SetNewMaxMp(const FP4WCharacterStat& InBaseStat, const FP4WCharacterStat& InModifierStat);

	UFUNCTION()
	void OnRep_CurrentHp();
	
	UFUNCTION()
	void OnRep_CurrentMp();
	
	UFUNCTION()
	void OnRep_CurrentExp();

	UFUNCTION()
	void OnRep_MaxHp();

	UFUNCTION()
	void OnRep_MaxMp();

	UFUNCTION()
	void OnRep_MaxExp();
	
	UFUNCTION()
	void OnRep_BaseStat();
	
	UFUNCTION()
	void OnRep_ModifierStat();

public:
	FOnHpZeroDelegate OnHpZero;
	FOnMpZeroDelegate OnMpZero;

	FOnHpChangedDelegate OnHpChanged;
	FOnMpChangedDelegate OnMpChanged;

	FOnStatChangedDelegate OnStatChanged;

	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE void AddBaseStat(const FP4WCharacterStat& InAddBaseStat) { BaseStat = BaseStat + InAddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetBaseStat(const FP4WCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE void SetModifierStat(const FP4WCharacterStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }

	FORCEINLINE void SetDamage(float ModifierDamage) { BaseStat.Attack = ModifierDamage; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }

	FORCEINLINE const FP4WCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FP4WCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE FP4WCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetMaxHp() const { return MaxHp; }

	FORCEINLINE float GetCurrentMp() const { return CurrentMp; }
	FORCEINLINE float GetMaxMp() const { return MaxMp; }

	FORCEINLINE float GetCurrentExp() const { return CurrentExp; }
	FORCEINLINE float GetMaxExp() const { return MaxExp; }

	FORCEINLINE void HealHp(float InHealAmount) { CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp, MaxHp); }

	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

	float ApplyDamage(float InDamage);

	float ApplyUseMp(float UseMp);

	void SetMpMax();

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
	float CurrentLevel;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHp, Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentMp, Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentMp;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentExp, Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentExp;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHp, Transient, VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	UPROPERTY(ReplicatedUsing = OnRep_MaxMp, Transient, VisibleInstanceOnly, Category = Stat)
	float MaxMp;

	UPROPERTY(ReplicatedUsing = OnRep_MaxExp, Transient, VisibleInstanceOnly, Category = Stat)
	float MaxExp;

	// ĳ������ �⺻ ����
	UPROPERTY(ReplicatedUsing = OnRep_BaseStat, Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FP4WCharacterStat BaseStat;
	
	// ����, ���, ... � ���� �߰��Ǵ� ����
	// ���� �ʿ� ���� ��
	UPROPERTY(ReplicatedUsing = OnRep_ModifierStat, Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FP4WCharacterStat ModifierStat;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

public:
	void ResetStat();
};
