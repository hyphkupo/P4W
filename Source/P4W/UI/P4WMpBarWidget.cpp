// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4WMpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/P4WCharacterWidgetInterface.h"

UP4WMpBarWidget::UP4WMpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UP4WMpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbMpBar")));
	ensure(MpProgressBar);

	IP4WCharacterWidgetInterface* CharacterWidget = Cast<IP4WCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UP4WMpBarWidget::UpdateMpBar(float NewCurrentMp, float NewMaxMp)
{
	CurrentMp = NewCurrentMp;
	MaxMp = NewMaxMp;

	ensure(MaxMp > 0.0f);
	if (MpProgressBar)
	{
		MpProgressBar->SetPercent(CurrentMp / MaxMp);
	}
}
