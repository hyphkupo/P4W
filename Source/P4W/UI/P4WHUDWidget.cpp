// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4WHUDWidget.h"
#include "UI/P4WHpBarWidget.h"
#include "Interface/P4WCharacterWidgetInterface.h"
#include "Interface/P4WCharacterHUDInterface.h"

UP4WHUDWidget::UP4WHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UP4WHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UP4WHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UP4WHpBarWidget>(GetWidgetFromName(TEXT("WidgetHPBar")));
	ensure(HpBar);

	IP4WCharacterHUDInterface* HUDPawn = Cast<IP4WCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
