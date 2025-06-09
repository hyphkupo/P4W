// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P4WCharacterPlayer_PLD.h"
#include "EnhancedInputComponent.h"
#include "Physics/P4WCollision.h"
#include "Interface/P4WAnimationAttackInterface.h"


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
}

// Cast: Instant, Cooldown time: 2.5s, MP Cost: 0MP, Range: 3y, Radius: 0y
// Damage: 220
void AP4WCharacterPlayer_PLD::Combo1Attack(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		CooldownTime = 2.5f;
		Stat->SetDamage(220);
	}
}

// Cast: Instant, Cooldown time: 2.5s, MP Cost: 0MP, Range: 3y, Radius: 0y
// Damage: 170 / if Combo Action 1, Combo Potency: 330
void AP4WCharacterPlayer_PLD::Combo2Attack(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		CooldownTime = 2.5f;
	}

}

// Cast: Instant, Cooldown time: 2.5s, MP Cost: 0MP, Range: 3y, Radius: 0y
// Damage: 100 / if Combo Action 1, Combo Potency: 330
void AP4WCharacterPlayer_PLD::Combo3Attack(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		CooldownTime = 2.5f;

	}

}

// Cast: 1.5s, Cooldown time: 2.5s, MP Cost: 2000MP, Range: 30y, Radius: 0y
// Cure Potency: 1000
void AP4WCharacterPlayer_PLD::HealUp(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		CooldownTime = 2.5f;

	}

}

// Cast: Instant, Cooldown time: 5s, MP Cost: 0MP, Range: 0y, Radius: 0y
// Reduces damage taken by 15%
// Duration: 6s, Oath Gauge Cost: 50
void AP4WCharacterPlayer_PLD::Sheltron(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		CooldownTime = 5.0f;

	}

}

// Cast: Instant, Cooldown time: 30s, MP Cost: 0MP, Range: 25y, Radius: 0y
// placing yourself at the top of a target's enmity list while gaining additional enmity
void AP4WCharacterPlayer_PLD::Provoke(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		CooldownTime = 30.0f;

	}

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

void AP4WCharacterPlayer_PLD::AttackHitCheck()
{
	// 서버
	if (IsLocallyControlled())
	{

	}

	// 클라이언트
	else if (!IsLocallyControlled())
	{

	}

	//if (IsLocallyControlled())
	//{
	//	FHitResult OutHitResult;
	//	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	//	const float AttackRange = Stat->GetTotalStat().AttackRange;
	//	const float AttackRadius = Stat->GetAttackRadius();
	//	const float AttackDamage = Stat->GetTotalStat().Attack;
	//	const FVector Forward = GetActorForwardVector();
	//	const FVector Start = GetActorLocation() + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
	//	const FVector End = Start + GetActorForwardVector() * AttackRange;

	//	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_P4WACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

	//	float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	//	if (!HasAuthority())
	//	{
	//		if (HitDetected)
	//		{
	//			ServerRPCNotifyHit(OutHitResult, HitCheckTime);
	//		}
	//		else
	//		{
	//			ServerRPCNotifyMiss(Start, End, Forward, HitCheckTime);
	//		}
	//	}
	//	else
	//	{
	//		FColor DebugColor = HitDetected ? FColor::Green : FColor::Red;
	//		DrawDebugAttackRange(DebugColor, Start, End, Forward);
	//		if (HitDetected)
	//		{
	//			AttackHitConfirm(OutHitResult.GetActor());
	//		}
	//	}
	//}
}

float AP4WCharacterPlayer_PLD::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}
