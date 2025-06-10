// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/P4WCharacterStatComponent.h"
#include "GameData/P4WGameSingleton.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UP4WCharacterStatComponent::UP4WCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 25.0f;

	bWantsInitializeComponent = true;
}

void UP4WCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	ResetStat();

	OnStatChanged.AddUObject(this, &UP4WCharacterStatComponent::SetNewMaxHp);
	OnStatChanged.AddUObject(this, &UP4WCharacterStatComponent::SetNewMaxMp);

	SetIsReplicated(true);
}

float UP4WCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
	UE_LOG(LogTemp, Log, TEXT("ActualDamage: %f"), ActualDamage);

	SetHp(PrevHp - ActualDamage);

	return ActualDamage;
}

void UP4WCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp, 0.0f, BaseStat.MaxHp);

	if (CurrentHp < KINDA_SMALL_NUMBER)
	{
		//CurrentHp = 0.0f;

		// 죽었다는 신호 전송
		OnHpZero.Broadcast();
	}

	OnHpChanged.Broadcast(CurrentHp, BaseStat.MaxHp);
}

void UP4WCharacterStatComponent::SetMp(float NewMp)
{
	CurrentMp = FMath::Clamp(NewMp, 0.0f, BaseStat.MaxMp);

	if (CurrentMp < KINDA_SMALL_NUMBER)
	{
		//CurrentHp = 0.0f;

		// 죽었다는 신호 전송
		OnMpZero.Broadcast();
	}

	OnMpChanged.Broadcast(CurrentMp, BaseStat.MaxMp);
}

void UP4WCharacterStatComponent::SetExp(float NewExp)
{
	CurrentExp = FMath::Clamp(NewExp, 0.0f, BaseStat.MaxExp);

	if (CurrentExp == BaseStat.MaxExp)
	{
		// @Todo: 레벨 업
		// @Todo: Exp 0으로 초기화
	}
}

// Called when the game starts
void UP4WCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	//SetHp(BaseStat.MaxHp);
	//SetMp(BaseStat.MaxMp);
	//SetExp(0.0f);			// 이후 저장한 경험치 불러오기
}

void UP4WCharacterStatComponent::ReadyForReplication()
{
	Super::ReadyForReplication();
}

void UP4WCharacterStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UP4WCharacterStatComponent, CurrentHp);
	DOREPLIFETIME(UP4WCharacterStatComponent, MaxHp);
	DOREPLIFETIME_CONDITION(UP4WCharacterStatComponent, BaseStat, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UP4WCharacterStatComponent, ModifierStat, COND_OwnerOnly);
}

void UP4WCharacterStatComponent::SetNewMaxHp(const FP4WCharacterStat& InBaseStat, const FP4WCharacterStat& InModifierStat)
{
	float PrevMaxHp = MaxHp;
	MaxHp = GetTotalStat().MaxHp;
	if (PrevMaxHp != MaxHp)
	{
		OnHpChanged.Broadcast(CurrentHp, MaxHp);
	}
}

void UP4WCharacterStatComponent::SetNewMaxMp(const FP4WCharacterStat& InBaseStat, const FP4WCharacterStat& InModifierStat)
{
	float PrevMaxMp = MaxMp;
	MaxMp = GetTotalStat().MaxMp;
	if (PrevMaxMp != MaxMp)
	{
		OnHpChanged.Broadcast(CurrentMp, MaxMp);
	}
}

void UP4WCharacterStatComponent::OnRep_CurrentHp()
{
	OnHpChanged.Broadcast(CurrentHp, MaxHp);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}

void UP4WCharacterStatComponent::OnRep_CurrentMp()
{
	OnMpChanged.Broadcast(CurrentMp, MaxMp);

	if (CurrentMp <= KINDA_SMALL_NUMBER)
	{
		OnMpZero.Broadcast();
	}
}

void UP4WCharacterStatComponent::OnRep_CurrentExp()
{
}

void UP4WCharacterStatComponent::OnRep_MaxHp()
{
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void UP4WCharacterStatComponent::OnRep_MaxMp()
{
	OnMpChanged.Broadcast(CurrentMp, MaxMp);
}

void UP4WCharacterStatComponent::OnRep_MaxExp()
{
	//OnExpChanged.Broadcast(CurrentHp, MaxExp);
}

void UP4WCharacterStatComponent::OnRep_BaseStat()
{
	// 스탯 변경 이벤트 발행
	OnStatChanged.Broadcast(BaseStat, ModifierStat);
}

void UP4WCharacterStatComponent::OnRep_ModifierStat()
{
	// 스탯 변경 이벤트 발행
	OnStatChanged.Broadcast(BaseStat, ModifierStat);
}

void UP4WCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UP4WGameSingleton::Get().CharacterMaxLevel);
	BaseStat = UP4WGameSingleton::Get().GetCharacterStat(CurrentLevel);
	check(BaseStat.MaxHp > 0.0f);
	check(BaseStat.MaxMp > 0.0f);
}

void UP4WCharacterStatComponent::ResetStat()
{
	SetLevelStat(CurrentLevel);
	MaxHp = BaseStat.MaxHp;
	MaxMp = BaseStat.MaxMp;
	MaxExp = BaseStat.MaxExp;
	SetHp(MaxHp);
	SetMp(MaxMp);
	SetExp(MaxExp);
}
