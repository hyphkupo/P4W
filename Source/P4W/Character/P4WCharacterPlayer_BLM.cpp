// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P4WCharacterPlayer_BLM.h"
#include "EnhancedInputComponent.h"

AP4WCharacterPlayer_BLM::AP4WCharacterPlayer_BLM()
{

}

void AP4WCharacterPlayer_BLM::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
	// Attack Section
		EnhancedInputComponent->BindAction(Combo1AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_BLM::BlizzardAttack);
		EnhancedInputComponent->BindAction(Combo2AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_BLM::FireAttack);
		EnhancedInputComponent->BindAction(Combo3AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_BLM::ThunderAttack);
		EnhancedInputComponent->BindAction(RAttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_BLM::FireBallAttack);
		EnhancedInputComponent->BindAction(FAttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_BLM::LeyLines);
	}
}

// Cast: 2s, Cooldown time: 2.5s, MP Cost: 400MP, Range: 25y, Radius: 0y
// Damage: 180
void AP4WCharacterPlayer_BLM::BlizzardAttack(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		bCanAttack = false;
		ComboNum = 1;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanAttack = true;
				}
			), 2.0f, false
		);
		PlayComboAttackAnimation(ComboNum);

		ServerRPCComboAttack(ComboNum);
	}
}

// Cast: 2s, Cooldown time: 2.5s, MP Cost: 800MP, Range: 25y, Radius: 0y
// Damage: 180
void AP4WCharacterPlayer_BLM::FireAttack(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		bCanAttack = false;
		ComboNum = 1;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanAttack = true;
				}
			), 2.0f, false
		);
		PlayComboAttackAnimation(ComboNum);

		ServerRPCComboAttack(ComboNum);
	}
}

// Cast: Instant, Cooldown time: 2.5s, MP Cost: 0MP, Range: 25y, Radius: 0y
// Damage: 100, Potency: 45, Duration: 24s
void AP4WCharacterPlayer_BLM::ThunderAttack(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		bCanAttack = false;
		ComboNum = 1;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		//FTimerHandle Handle;
		//GetWorld()->GetTimerManager().SetTimer(
		//	Handle,
		//	FTimerDelegate::CreateLambda([&]()
		//		{
		//			bCanAttack = true;
		//		}
		//	), 2.0f, false
		//);
		PlayComboAttackAnimation(ComboNum);

		ServerRPCComboAttack(ComboNum);
	}
}

// Cast: 3.5s, Cooldown time: 2.5s, MP Cost: 800MP, Range: 25y, Radius: 0y
// Damage: 300
void AP4WCharacterPlayer_BLM::FireBallAttack(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		bCanAttack = false;
		ComboNum = 1;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanAttack = true;
				}
			), 2.5f, false
		);
		PlayComboAttackAnimation(ComboNum);

		ServerRPCComboAttack(ComboNum);
	}
}

// Cast: Instant, Cooldown time: 120s, MP Cost: 0MP, Range: 0y, Radius: 3y
// while standing within it, reduces spell cast time and recast time, and auto-attack delay by 15%
// Duration: 20s, Maximum Charges: 2
void AP4WCharacterPlayer_BLM::LeyLines(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		bCanAttack = false;
		ComboNum = 1;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		//FTimerHandle Handle;
		//GetWorld()->GetTimerManager().SetTimer(
		//	Handle,
		//	FTimerDelegate::CreateLambda([&]()
		//		{
		//			bCanAttack = true;
		//		}
		//	), 2.0f, false
		//);
		PlayComboAttackAnimation(ComboNum);

		ServerRPCComboAttack(ComboNum);
	}
}

void AP4WCharacterPlayer_BLM::SetupHUDWidget(UP4WHUDWidget* InHUDWidget)
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
