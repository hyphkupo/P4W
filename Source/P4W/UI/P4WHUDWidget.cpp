// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4WHUDWidget.h"
#include "UI/P4WHpBarWidget.h"
#include "UI/P4WMpBarWidget.h"
#include "UI/P4WCooltimeWidget.h"
#include "UI/P4WCharacterStatWidget.h"
#include "Interface/P4WCharacterWidgetInterface.h"
#include "Interface/P4WCharacterHUDInterface.h"
#include "Components/ProgressBar.h"

UP4WHUDWidget::UP4WHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UP4WHUDWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	HpBar->UpdateHpBar(NewCurrentHp, NewMaxHp);
}

void UP4WHUDWidget::UpdateMpBar(float NewCurrentMp, float NewMaxMp)
{
	MpBar->UpdateMpBar(NewCurrentMp, NewMaxMp);
}

void UP4WHUDWidget::UpdateExpBar(float NewCurrentExp, float NewMaxExp)
{
}

//void UP4WHUDWidget::TickCooltime()
//{
//	CurrentCooltime -= 0.05f;
//	if (CurrentCooltime <= 0.0f)
//	{
//		CurrentCooltime = 0.0f;
//		GetWorld()->GetTimerManager().ClearTimer(CooltimeTimerHandle);
//	}
//	UpdateCooltimeBar();
//}
//
//void UP4WHUDWidget::UpdateCooltimeBar()
//{
//	if (PbCooltimeBar && MaxCooltime > 0.0f)
//	{
//		PbCooltimeBar->SetPercent(CurrentCooltime / MaxCooltime);
//	}
//}

void UP4WHUDWidget::UpdateStat(const FP4WCharacterStat& BaseStat, const FP4WCharacterStat& ModifierStat)
{
	FP4WCharacterStat TotalStat = BaseStat + ModifierStat;
	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

//void UP4WHUDWidget::StartCooltime(float Duration)
//{
//	if (CooltimeBar)
//	{
//		CooltimeBar->StartCooltime(Duration);
//	}
//}

void UP4WHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UP4WHpBarWidget>(GetWidgetFromName(TEXT("WidgetHPBar")));
	ensure(HpBar);

	MpBar = Cast<UP4WMpBarWidget>(GetWidgetFromName(TEXT("WidgetMPBar")));
	ensure(MpBar);

	CooltimeBar = Cast<UP4WCooltimeWidget>(GetWidgetFromName(TEXT("WidgetCooltimeBar2")));
	ensure(CooltimeBar);

	//CharacterStat = Cast<UP4WCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	//ensure(CharacterStat);

	IP4WCharacterHUDInterface* HUDPawn = Cast<IP4WCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
