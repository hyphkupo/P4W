// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4WHUDWidget.h"
#include "UI/P4WHpBarWidget.h"
#include "UI/P4WCharacterStatWidget.h"
#include "Interface/P4WCharacterWidgetInterface.h"
#include "Interface/P4WCharacterHUDInterface.h"

UP4WHUDWidget::UP4WHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UP4WHUDWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UP4WHUDWidget::UpdateMpBar(float NewCurrentMp, float NewMaxMp)
{
}

void UP4WHUDWidget::UpdateExpBar(float NewCurrentExp, float NewMaxExp)
{
}

void UP4WHUDWidget::UpdateStat(const FP4WCharacterStat& BaseStat, const FP4WCharacterStat& ModifierStat)
{
	FP4WCharacterStat TotalStat = BaseStat + ModifierStat;
	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UP4WHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UP4WHpBarWidget>(GetWidgetFromName(TEXT("WidgetHPBar")));
	ensure(HpBar);

	//CharacterStat = Cast<UP4WCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	//ensure(CharacterStat);

	IP4WCharacterHUDInterface* HUDPawn = Cast<IP4WCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
