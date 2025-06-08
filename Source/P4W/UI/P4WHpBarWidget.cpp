// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4WHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/P4WCharacterWidgetInterface.h"

UP4WHpBarWidget::UP4WHpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UP4WHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	IP4WCharacterWidgetInterface* CharacterWidget = Cast<IP4WCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UP4WHpBarWidget::UpdateHpBar(float NewCurrentHp, float NewMaxHp)
{
	CurrentHp = NewCurrentHp;
	MaxHp = NewMaxHp;

	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
		UE_LOG(LogTemp, Log, TEXT("HpPercent: %f"), CurrentHp / MaxHp);
	}
}
