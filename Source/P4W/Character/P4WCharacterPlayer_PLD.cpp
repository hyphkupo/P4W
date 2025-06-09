// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P4WCharacterPlayer_PLD.h"
#include "EnhancedInputComponent.h"
#include "Physics/P4WCollision.h"
#include "Interface/P4WAnimationAttackInterface.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameStateBase.h"


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

bool AP4WCharacterPlayer_PLD::ServerRPCNotifyHit_Validate(const FHitResult& HitResult, float HitCheckTime)
{
	return true;
}

void AP4WCharacterPlayer_PLD::ServerRPCNotifyHit_Implementation(const FHitResult& HitResult, float HitCheckTime)
{
//	AActor* HitActor = HitResult.GetActor();
//	if (::IsValid(HitActor))
//	{
//		const FVector HitLocation = HitResult.Location;
//		const FBox HitBox = HitActor->GetComponentsBoundingBox();
//		const FVector ActorBoxCenter = (HitBox.Min + HitBox.Max) * 0.5f;
//		if (FVector::DistSquared(HitLocation, ActorBoxCenter) <= AttackCheckDistance * AttackCheckDistance)
//		{
//			AttackHitConfirm(HitActor);
//		}
//		else
//		{
//			AB_LOG(LogABNetwork, Warning, TEXT("%s"), TEXT("HitTest Rejected!"));
//		}
//
//#if ENABLE_DRAW_DEBUG
//		// ���� ������ ��ġ�� �ϴû����� ǥ�� (������).
//		DrawDebugPoint(GetWorld(), ActorBoxCenter, 30.0f, FColor::Cyan, false, 5.0f);
//
//		// ���� ������ ��ġ�� ��ȫ������ ǥ�� (������).
//		DrawDebugPoint(GetWorld(), HitLocation, 30.0f, FColor::Magenta, false, 5.0f);
//
//#endif
//		// ����� ��ο�� ���� ���� �����ֱ�.
//		DrawDebugAttackRange(FColor::Green, HitResult.TraceStart, HitResult.TraceEnd, HitActor->GetActorForwardVector());
//	}
}

bool AP4WCharacterPlayer_PLD::ServerRPCNotifyMiss_Validate(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	return true;
}

void AP4WCharacterPlayer_PLD::ServerRPCNotifyMiss_Implementation(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
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

void AP4WCharacterPlayer_PLD::AttackHitCheck()
{
	Super::AttackHitCheck();

//	// ���� �������� ���� Ŭ���̾�Ʈ���� ������ ����
//	if (IsLocallyControlled())
//	{
//		// �浹�ߴ��� ����
//		FHitResult OutHitResult;
//		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
//
//		const float AttackRange = Stat->GetTotalStat().AttackRange;
//		const float AttackRadius = Stat->GetAttackRadius();
//		const float AttackDamage = Stat->GetTotalStat().Attack;
//		const FVector Forward = GetActorForwardVector();
//		const FVector Start = GetActorLocation() + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
//		const FVector End = Start + Forward * AttackRange;
//
//		bool HitDetected = GetWorld()->SweepSingleByChannel(
//			OutHitResult,
//			Start,
//			End,
//			FQuat::Identity,
//			CCHANNEL_P4WACTION,
//			FCollisionShape::MakeSphere(AttackRadius),
//			Params
//		);
//
//		// ���� ���� Ÿ�̹�
//		//float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
//
//		// Ŭ���̾�Ʈ���� ����
//		// Ŭ���̾�Ʈ���� ������ �����ϴ� ��쿡�� �ٽ� ���� �ʿ� ��Ŷ�� ������ ���� ������ ����� ���� ������ �޾ƾ� ��
//		if (!HasAuthority())
//		{
//			// ������ �ð��� �����ͼ� ���ڷ� �ѱ� ��
//			float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
//			// ������ ���ؼ� ������ ���� RPC�� ��� ���Ͽ� ����
//
//			if (HitDetected)
//			{
//				ServerRPCNotifyHit(OutHitResult, HitCheckTime);
//			}
//			else
//			{
//				ServerRPCNotifyMiss(Start, End, Forward, HitCheckTime);
//			}
//		}
//
//		// �������� ���� (������ �÷��̾� �� ����, ���� ��Ŷ�� ���� �ʿ䰡 �����Ƿ� �ٷ� ó���ϸ� ��)
//		else
//		{
//			FColor DebugColor = HitDetected ? FColor::Green : FColor::Red;
//			DrawDebugAttackRange(DebugColor, Start, End, Forward);
//
//			// ������ ��쿡�� �������� �ʰ� �״�� ����� ���� ���� ����.
//			if (HitDetected)
//			{
//				AttackHitConfirm(OutHitResult.GetActor());
//			}
//		}
//#if 0
//		if (HitDetected)
//		{
//			FDamageEvent DamageEvent;
//			OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
//		}
//
//#if ENABLE_DRAW_DEBUG
//
//		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//		float CapsuleHalfHeight = AttackRange * 0.5f;
//		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//
//		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
//
//#endif
//#endif
//	}
}

	/*
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
	*/

float AP4WCharacterPlayer_PLD::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}
