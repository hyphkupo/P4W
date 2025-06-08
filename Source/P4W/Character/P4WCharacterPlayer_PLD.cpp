// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P4WCharacterPlayer_PLD.h"
#include "EnhancedInputComponent.h"

AP4WCharacterPlayer_PLD::AP4WCharacterPlayer_PLD()
{
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT(""))

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboAttackMontageRef(TEXT("/Game/Animation/AM_PLDCombo.AM_PLDCombo"));
	if (ComboAttackMontageRef.Object)
	{
		ComboAttackMontage = ComboAttackMontageRef.Object;
	}
}

void AP4WCharacterPlayer_PLD::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	//if (EnhancedInputComponent)
	//{
	//// Attack Section
	//	// Combo Attack
	//	EnhancedInputComponent->BindAction(Combo1AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_PLD::Combo1Attack);
	//	EnhancedInputComponent->BindAction(Combo2AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_PLD::Combo2Attack);
	//	EnhancedInputComponent->BindAction(Combo3AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_PLD::Combo3Attack);
	//}
}

void AP4WCharacterPlayer_PLD::Combo1Attack(const FInputActionValue& Value)
{

}

void AP4WCharacterPlayer_PLD::Combo2Attack(const FInputActionValue& Value)
{
}

void AP4WCharacterPlayer_PLD::Combo3Attack(const FInputActionValue& Value)
{
}

void AP4WCharacterPlayer_PLD::SetupHUDWidget(UP4WHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp(), Stat->GetMaxHp());
		InHUDWidget->UpdateMpBar(Stat->GetCurrentMp(), Stat->GetMaxMp());

		Stat->OnStatChanged.AddUObject(InHUDWidget, &UP4WHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UP4WHUDWidget::UpdateHpBar);
		Stat->OnMpChanged.AddUObject(InHUDWidget, &UP4WHUDWidget::UpdateMpBar);
	}
}
