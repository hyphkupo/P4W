// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4WHpBarWidget.h"
#include "Components/ProgressBar.h"

UP4WHpBarWidget::UP4WHpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxHp = 200.0f;
}

void UP4WHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	UpdateHpBar(200.0f);
}

void UP4WHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
