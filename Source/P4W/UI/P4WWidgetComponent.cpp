// Fill out your copyright notice in the Description page of Project Settings.


#include "P4WWidgetComponent.h"
#include "UI/P4WUserWidget.h"

void UP4WWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UP4WUserWidget* P4WUserWidget = Cast<UP4WUserWidget>(GetWidget());
	if (P4WUserWidget)
	{
		P4WUserWidget->SetOwningActor(GetOwner());
	}
}
