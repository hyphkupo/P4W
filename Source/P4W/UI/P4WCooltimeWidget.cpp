// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/P4WCooltimeWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/P4WCharacterWidgetInterface.h"

UP4WCooltimeWidget::UP4WCooltimeWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxCooltime = 1.0f;
	CurrentCooltime = 1.0f;
}

void UP4WCooltimeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//CooltimeProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbCooltimeBar")));
	//ensure(CooltimeProgressBar);

	//IP4WCharacterWidgetInterface* CharacterWidget = Cast<IP4WCharacterWidgetInterface>(OwningActor);
	//if (CharacterWidget)
	//{
	//	CharacterWidget->SetupCharacterWidget(this);
	//}

	UE_LOG(LogTemp, Warning, TEXT("CooltimeBar ptr: %p"), PbCooltimeBar);
	check(PbCooltimeBar);
}

void UP4WCooltimeWidget::StartCooltime(float InCooltime)
{
	//if (!PbCooltimeBar)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("CooltimeBar is null! Make sure UMG binding is correct."));
	//	return;
	//}

	//MaxCooltime = InCooltime;
	//CurrentCooltime = InCooltime;

	//UpdateCooltimeBar();

	//GetWorld()->GetTimerManager().SetTimer(
	//	CooltimeTimerHandle,
	//	this,
	//	&UP4WCooltimeWidget::TickCooltime,
	//	0.05f,
	//	true
	//);
}

void UP4WCooltimeWidget::TickCooltime()
{
	CurrentCooltime -= 0.05f;
	if (CurrentCooltime <= 0.0f)
	{
		CurrentCooltime = 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(CooltimeTimerHandle);
	}
	PbCooltimeBar->SetPercent(CurrentCooltime / MaxCooltime);
	UpdateCooltimeBar();
}

void UP4WCooltimeWidget::UpdateCooltimeBar()
{
	if (PbCooltimeBar && MaxCooltime > 0.0f)
	{
		PbCooltimeBar->SetPercent(CurrentCooltime / MaxCooltime);
	}
}
