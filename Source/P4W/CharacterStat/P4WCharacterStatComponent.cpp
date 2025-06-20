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

float UP4WCharacterStatComponent::ApplyUseMp(float UseMp)
{
	const float PrevMp = CurrentMp;
	const float ActualMp = FMath::Clamp<float>(UseMp, 0, UseMp);
	UE_LOG(LogTemp, Log, TEXT("UseMp: %f"), UseMp);

	SetMp(PrevMp - ActualMp);

	return ActualMp;
}

void UP4WCharacterStatComponent::SetMpMax()
{
	SetMp(MaxMp);
}

float UP4WCharacterStatComponent::UpdateEnmity(float ChangedEnmity)
{
	const float NewEnmity = FMath::Clamp<float>(CurrentEnmity + ChangedEnmity, 0, 1000.0f);
	UE_LOG(LogTemp, Log, TEXT("CurrentEnmity: %f"), NewEnmity);

	SetEnmity(NewEnmity);

	return NewEnmity;
}

void UP4WCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp, 0.0f, BaseStat.MaxHp);

	if (CurrentHp < KINDA_SMALL_NUMBER)
	{
		//CurrentHp = 0.0f;

		// �׾��ٴ� ��ȣ ����
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

		// �׾��ٴ� ��ȣ ����
		OnMpZero.Broadcast();
	}

	OnMpChanged.Broadcast(CurrentMp, BaseStat.MaxMp);
}

void UP4WCharacterStatComponent::SetExp(float NewExp)
{
	CurrentExp = FMath::Clamp(NewExp, 0.0f, BaseStat.MaxExp);

	if (CurrentExp == BaseStat.MaxExp)
	{
		// @Todo: ���� ��
		// @Todo: Exp 0���� �ʱ�ȭ
	}
}

void UP4WCharacterStatComponent::SetEnmity(float NewEnmity)
{
	CurrentEnmity = FMath::Clamp(NewEnmity, 0.0f, 1000.0f);

	OnEnmityChanged.Broadcast(CurrentEnmity);
}

// Called when the game starts
void UP4WCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	//SetHp(BaseStat.MaxHp);
	//SetMp(BaseStat.MaxMp);
	//SetExp(0.0f);			// ���� ������ ����ġ �ҷ�����
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
	DOREPLIFETIME(UP4WCharacterStatComponent, CurrentMp);
	DOREPLIFETIME(UP4WCharacterStatComponent, MaxMp);
	DOREPLIFETIME(UP4WCharacterStatComponent, CurrentExp);
	DOREPLIFETIME(UP4WCharacterStatComponent, MaxExp);
	DOREPLIFETIME(UP4WCharacterStatComponent, CurrentEnmity);
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
	OnExpChanged.Broadcast(CurrentExp, MaxExp);

	if (CurrentExp >= MaxExp)
	{
		OnExpMax.Broadcast();
	}
}

void UP4WCharacterStatComponent::OnRep_CurrentEnmity()
{
	OnEnmityChanged.Broadcast(CurrentEnmity);
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
	OnExpChanged.Broadcast(CurrentExp, MaxExp);
}

void UP4WCharacterStatComponent::OnRep_BaseStat()
{
	// ���� ���� �̺�Ʈ ����
	OnStatChanged.Broadcast(BaseStat, ModifierStat);
}

void UP4WCharacterStatComponent::OnRep_ModifierStat()
{
	// ���� ���� �̺�Ʈ ����
	OnStatChanged.Broadcast(BaseStat, ModifierStat);
}

void UP4WCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UP4WGameSingleton::Get().CharacterMaxLevel);
	BaseStat = UP4WGameSingleton::Get().GetCharacterStat(CurrentLevel);
	check(BaseStat.MaxHp > 0.0f);
	check(BaseStat.MaxMp > 0.0f);
	check(BaseStat.MaxExp >= 0.0f);
}

void UP4WCharacterStatComponent::ResetStat()
{
	SetLevelStat(CurrentLevel);
	MaxHp = BaseStat.MaxHp;
	MaxMp = BaseStat.MaxMp;
	MaxExp = BaseStat.MaxExp;
	SetHp(MaxHp);
	SetMp(MaxMp);
	SetExp(0);
	SetEnmity(0);
}
