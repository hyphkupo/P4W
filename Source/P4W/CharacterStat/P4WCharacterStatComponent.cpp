// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/P4WCharacterStatComponent.h"

// Sets default values for this component's properties
UP4WCharacterStatComponent::UP4WCharacterStatComponent()
{
	MaxHp = 200.0f;
	CurrentHp = MaxHp;
}

// Called when the game starts
void UP4WCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
}

float UP4WCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);

	return ActualDamage;
}

void UP4WCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp, 0.0f, MaxHp);

	if (CurrentHp < KINDA_SMALL_NUMBER)
	{
		//CurrentHp = 0.0f;

		// 죽었다는 신호 전송
		OnHpZero.Broadcast();
	}

	OnHpChanged.Broadcast(CurrentHp);
}

float UP4WCharacterStatComponent::GetCurrentHp()
{
	return CurrentHp;
}

float UP4WCharacterStatComponent::GetMaxHp()
{
	return MaxHp;
}

//void UP4WCharacterStatComponent::SetJobStat(int32 InNewLevel)
//{
//
//}
