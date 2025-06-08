// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/P4WPlayerController.h"
#include "UI/P4WHUDWidget.h"
#include "UI/P4WHpBarWidget.h"
#include "CharacterStat/P4WCharacterStatComponent.h"

AP4WPlayerController::AP4WPlayerController()
{
	static ConstructorHelpers::FClassFinder<UP4WHUDWidget> P4WHUDWidgetRef(TEXT("/Game/UI/WBP_P4WHUD.WBP_P4WHUD_C"));
	if (P4WHUDWidgetRef.Class)
	{
		P4WHUDWidgetClass = P4WHUDWidgetRef.Class;
	}
}

void AP4WPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AP4WPlayerController::PostNetInit()
{
	Super::PostNetInit();

	UNetDriver* NetDriver = GetNetDriver();
}

void AP4WPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	//P4WHUDWidget = CreateWidget<UP4WHUDWidget>(this, P4WHUDWidgetClass);
	//if (P4WHUDWidget)
	//{
	//	SetupHUDWidget(P4WHUDWidget);
	//	P4WHUDWidget->AddToViewport();
	//}
}

void AP4WPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

//void AP4WPlayerController::SetupHUDWidget(UP4WHUDWidget* InHUDWidget)
//{
//
//	//if (InHUDWidget)
//	//{
//	//	//InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
//	//	//InHUDWidget->UpdateHpBar(Stat->GetMaxHp());
//
//	//	////Stat->OnStatChanged.AddUObject(InHUDWidget, &UP4WHUDWidget::UpdateStat);
//	//	//Stat->OnHpChanged.AddUObject(InHUDWidget, &UP4WHUDWidget::UpdateHpBar);
//	//}
//}
