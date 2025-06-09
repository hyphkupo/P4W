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
//		// 맞은 액터의 위치를 하늘색으로 표시 (점으로).
//		DrawDebugPoint(GetWorld(), ActorBoxCenter, 30.0f, FColor::Cyan, false, 5.0f);
//
//		// 맞은 액터의 위치를 자홍색으로 표시 (점으로).
//		DrawDebugPoint(GetWorld(), HitLocation, 30.0f, FColor::Magenta, false, 5.0f);
//
//#endif
//		// 디버그 드로우로 공격 영역 보여주기.
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

//	// 지금 소유권을 가진 클라이언트에서 판정을 진행
//	if (IsLocallyControlled())
//	{
//		// 충돌했는지 판정
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
//		// 공격 판정 타이밍
//		//float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
//
//		// 클라이언트에서 진행
//		// 클라이언트에서 판정을 진행하는 경우에는 다시 서버 쪽에 패킷을 보내서 내가 판정한 결과에 대한 검증을 받아야 함
//		if (!HasAuthority())
//		{
//			// 서버의 시간을 가져와서 인자로 넘길 것
//			float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
//			// 판정을 위해서 서버로 보낼 RPC를 헤더 파일에 선언
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
//		// 서버에서 진행 (서버의 플레이어 때 판정, 굳이 패킷을 보낼 필요가 없으므로 바로 처리하면 됨)
//		else
//		{
//			FColor DebugColor = HitDetected ? FColor::Green : FColor::Red;
//			DrawDebugAttackRange(DebugColor, Start, End, Forward);
//
//			// 서버인 경우에는 판정하지 않고 그대로 대미지 전달 로직 수행.
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
