// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P4WCharacterPlayer_BLM.h"
#include "EnhancedInputComponent.h"
#include "Physics/P4WCollision.h"
#include "Components/CapsuleComponent.h"
//#include "P4WCharacterPlayer_PLD.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "P4W.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/P4WPLDComboActionData.h"

#include "UI/P4WUserWidget.h"
#include "UI/P4WHpBarWidget.h"
#include "UI/P4WMpBarWidget.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"

AP4WCharacterPlayer_BLM::AP4WCharacterPlayer_BLM()
{
	// Attack Input
	static ConstructorHelpers::FObjectFinder<UInputAction> AutoAttackActionRef(TEXT("/Game/Input/IA_AutoAttack.IA_AutoAttack"));
	if (AutoAttackActionRef.Object)
	{
		AutoAttackAction = AutoAttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Combo1AttackActionRef(TEXT("/Game/Input/IA_Combo1Attack.IA_Combo1Attack"));
	if (Combo1AttackActionRef.Object)
	{
		Combo1AttackAction = Combo1AttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Combo2AttackActionRef(TEXT("/Game/Input/IA_Combo2Attack.IA_Combo2Attack"));
	if (Combo2AttackActionRef.Object)
	{
		Combo2AttackAction = Combo2AttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Combo3AttackActionRef(TEXT("/Game/Input/IA_Combo3Attack.IA_Combo3Attack"));
	if (Combo3AttackActionRef.Object)
	{
		Combo3AttackAction = Combo3AttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RAttackActionRef(TEXT("/Game/Input/IA_RAttack.IA_RAttack"));
	if (RAttackActionRef.Object)
	{
		RAttackAction = RAttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> FAttackActionRef(TEXT("/Game/Input/IA_FAttack.IA_FAttack"));
	if (FAttackActionRef.Object)
	{
		FAttackAction = FAttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AnyKeyPressedActionRef(TEXT("/Game/Input/IA_AnyKeyPressed.IA_AnyKeyPressed"));
	if (AnyKeyPressedActionRef.Object)
	{
		AnyKeyPressedAction = AnyKeyPressedActionRef.Object;
	}

	// Animation Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAttackMontageRef(TEXT("/Game/Animation/AM_FireAttack.AM_FireAttack"));
	if (FireAttackMontageRef.Object)
	{
		FireAttackMontage = FireAttackMontageRef.Object;
	}

	bReplicates = true;
	bCanAttack = true;
	bIsAnyKeyPressed = false;
	bIsCasting = false;
	CastingTime = 0.0f;
}

void AP4WCharacterPlayer_BLM::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AP4WCharacterPlayer_BLM, CastingTime);
}

bool AP4WCharacterPlayer_BLM::CheckWasMappingKeyPressed()
{
	TArray<FInputAxisKeyMapping> axisMapping;
	//UInputSettings::GetInputSettings()->GetAxisMappingByName(axisName, axisMapping);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		for (auto axis : axisMapping)
		{
			if (PlayerController->WasInputKeyJustPressed(axis.Key))
			{
				return true;
			}
		}
	}
	
	return false;
}

void AP4WCharacterPlayer_BLM::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		// Attack Section
		EnhancedInputComponent->BindAction(AutoAttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_BLM::AutoAttack);

		EnhancedInputComponent->BindAction(Combo1AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_BLM::BlizzardAttack);
		EnhancedInputComponent->BindAction(Combo2AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_BLM::FireAttack);
		EnhancedInputComponent->BindAction(Combo3AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_BLM::ThunderAttack);
		EnhancedInputComponent->BindAction(RAttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_BLM::FireBallAttack);
		EnhancedInputComponent->BindAction(FAttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_BLM::Manafont);

		EnhancedInputComponent->BindAction(AnyKeyPressedAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_BLM::AnyKeyPressed);
	}
}

void AP4WCharacterPlayer_BLM::AutoAttack(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		bCanAttack = false;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanAttack = true;
				}
			), 1.5f, false
		);
		PlayAutoAttackAnimation();

		ServerRPCAutoAttack();
	}
}

// Cast: 2s, Cooldown time: 2.5s, MP Cost: 400MP, Range: 25y, Radius: 0y
// Damage: 180
void AP4WCharacterPlayer_BLM::BlizzardAttack(const FInputActionValue& Value)
{
	// 타겟이 없으면 공격 불가
	if (!HitTarget)
	{
		if (IsLocallyControlled())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("공격할 대상이 없습니다"));
			return;
		}

		//APlayerController* PlayerController = Cast<APlayerController>(GetController());
		//if (PlayerController)
		//{
		//	PlayerController->ClientMessage(TEXT("공격할 대상이 없습니다"));
		//	return;
		//}
	}

	Stat->ApplyUseMp(600.0f);

	// 애니메이션 재생
	// 2초 전에 다른 입력을 받으면 애니메이션 정지
	// 재생 후 2초가 지나면(그 안에 다른 입력을 받지 않으면) 노티파이 이용해서 공격, 이펙트 재생, 대미지 적용

	//if (bCanAttack)
	//{
	//	bCanAttack = false;

	//	// 2초 후 대미지 줌, 공격 이펙트 재생

	//	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	//	FTimerHandle Handle;
	//	GetWorld()->GetTimerManager().SetTimer(
	//		Handle,
	//		FTimerDelegate::CreateLambda([&]()
	//			{
	//				bCanAttack = true;
	//			}
	//		), 2.0f, false
	//	);
	//	PlayComboAttackAnimation(ComboNum);

	//	ServerRPCComboAttack(ComboNum);
	//}
}

// Cast: 2s, Cooldown time: 2.5s, MP Cost: 800MP, Range: 25y, Radius: 0y
// Damage: 180
void AP4WCharacterPlayer_BLM::FireAttack(const FInputActionValue& Value)
{
	CastingTime = 2.0f;

	// 타겟이 없으면 공격 불가
	if (!HitTarget)
	{
		if (IsLocallyControlled())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("공격할 대상이 없습니다"));
			return;
		}
	}

	Stat->ApplyUseMp(800.0f);
	
	if (bCanAttack)
	{
		CooldownTime = 2.5f;

		bIsCasting = true;

		//PlayFireAttackAnimation();
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		PlayFireAttackAnimation(CastingTime);

		UE_LOG(LogTemp, Log, TEXT("bIsCasting: %d, bIsAnyKeyPressed: %d"), bIsCasting, bIsAnyKeyPressed);

		if (bIsCasting && bIsAnyKeyPressed)
		{
			UE_LOG(LogTemp, Log, TEXT("bIsCasting: %d, bIsAnyKeyPressed: %d"), bIsCasting, bIsAnyKeyPressed);
			//AnimInstance->StopAllMontages(0.5f);
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Stop(0.0f, FireAttackMontage);

			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}

		ServerRPCFireAttack(CastingTime);
	}
}

// Cast: Instant, Cooldown time: 2.5s, MP Cost: 0MP, Range: 25y, Radius: 0y
// Damage: 100, Potency: 45, Duration: 24s
void AP4WCharacterPlayer_BLM::ThunderAttack(const FInputActionValue& Value)
{
	// 타겟이 없으면 공격 불가
	if (!HitTarget)
	{
		if (IsLocallyControlled())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("공격할 대상이 없습니다"));
			return;
		}
	}


	//if (bCanAttack)
	//{
	//	bCanAttack = false;
	//	ComboNum = 1;

	//	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	//	//FTimerHandle Handle;
	//	//GetWorld()->GetTimerManager().SetTimer(
	//	//	Handle,
	//	//	FTimerDelegate::CreateLambda([&]()
	//	//		{
	//	//			bCanAttack = true;
	//	//		}
	//	//	), 2.0f, false
	//	//);
	//	PlayComboAttackAnimation(ComboNum);

	//	ServerRPCComboAttack(ComboNum);
	//}
}

// RAttackAction
// Cast: 3.5s, Cooldown time: 2.5s, MP Cost: 800MP, Range: 25y, Radius: 0y
// Damage: 300
void AP4WCharacterPlayer_BLM::FireBallAttack(const FInputActionValue& Value)
{
	// 타겟이 없으면 공격 불가
	if (!HitTarget)
	{
		if (IsLocallyControlled())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("공격할 대상이 없습니다"));
			return;
		}
	}

	Stat->ApplyUseMp(800.0f);

	//if (bCanAttack)
	//{
	//	bCanAttack = false;
	//	ComboNum = 1;

	//	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	//	FTimerHandle Handle;
	//	GetWorld()->GetTimerManager().SetTimer(
	//		Handle,
	//		FTimerDelegate::CreateLambda([&]()
	//			{
	//				bCanAttack = true;
	//			}
	//		), 2.5f, false
	//	);
	//	PlayComboAttackAnimation(ComboNum);

	//	ServerRPCComboAttack(ComboNum);
	//}
}

// FAttackAction
// Cast: Instant, Cooldown time: 120s, MP Cost: 0MP, Range: 0y, Radius: 0y
// Fully restores MP.
void AP4WCharacterPlayer_BLM::Manafont(const FInputActionValue& Value)
{
	Stat->SetMpMax();
}

void AP4WCharacterPlayer_BLM::AnyKeyPressed(const FInputActionValue& Value)
{
	bIsAnyKeyPressed = true;

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda([&]()
			{
				bIsAnyKeyPressed = false;
			}
		), 1.0f, false
	);
}

void AP4WCharacterPlayer_BLM::PlayAutoAttackAnimation()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AutoAttackMontage);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda([&]()
			{
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			}
		), 1.0f, false
	);
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

void AP4WCharacterPlayer_BLM::DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
#if ENABLE_DRAW_DEBUG

	const float AttackRange = Stat->GetTotalStat().AttackRange;
	const float AttackRadius = Stat->GetAttackRadius();
	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);

#endif
}

bool AP4WCharacterPlayer_BLM::ServerRPCNotifyHit_Validate(const FHitResult& HitResult, float HitCheckTime)
{
	return true;
}

void AP4WCharacterPlayer_BLM::ServerRPCNotifyHit_Implementation(const FHitResult& HitResult, float HitCheckTime)
{
	UE_LOG(LogTemp, Log, TEXT("[%s]CurrentDamage: %f"), LOG_NETMODEINFO, CurrentDamage);
	AActor* HitActor = HitResult.GetActor();
	if (::IsValid(HitActor))
	{
		const FVector HitLocation = HitResult.Location;
		const FBox HitBox = HitActor->GetComponentsBoundingBox();
		const FVector ActorBoxCenter = (HitBox.Min + HitBox.Max) * 0.5f;
		if (FVector::DistSquared(HitLocation, ActorBoxCenter) <= AcceptCheckDistance * AcceptCheckDistance)
		{
			if (HasAuthority())
			{
				//UE_LOG(LogTemp, Log, TEXT("[%s]CurrentDamage In If: %f"), LOG_NETMODEINFO, CurrentDamage);
				//float AttackDamage;
				if (bIsInCombo)
				{
					//AttackDamage = CurrentDamage;
					PrevDamage = Stat->GetTotalStat().Attack;
					//UE_LOG(LogTemp, Log, TEXT("[%s]CurrentDamage In If: %f"), LOG_NETMODEINFO, CurrentDamage);
					Stat->SetDamage(CurrentDamage);

				}
				else
				{
					//UE_LOG(LogTemp, Log, TEXT("[%s]CurrentDamage In If: %f"), LOG_NETMODEINFO, CurrentDamage);
					//AttackDamage = Stat->GetTotalStat().Attack;
					//Stat->SetDamage(30.0f);
				}
				FDamageEvent DamageEvent;
				HitActor->TakeDamage(Stat->GetTotalStat().Attack, DamageEvent, GetController(), this);
				UE_LOG(LogTemp, Log, TEXT("AttackDamage: %f"), Stat->GetTotalStat().Attack);

				// 기본 공격 대미지로 되돌림
				Stat->SetDamage(PrevDamage);
			}
		}
		else
		{
		}

#if ENABLE_DRAW_DEBUG
		//DrawDebugPoint(GetWorld(), ActorBoxCenter, 50.0f, FColor::Cyan, false, 5.0f);
		//DrawDebugPoint(GetWorld(), HitLocation, 50.0f, FColor::Magenta, false, 5.0f);
#endif

		DrawDebugAttackRange(FColor::Green, HitResult.TraceStart, HitResult.TraceEnd, HitActor->GetActorForwardVector());
	}
}

bool AP4WCharacterPlayer_BLM::ServerRPCNotifyMiss_Validate(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	return true;
}

void AP4WCharacterPlayer_BLM::ServerRPCNotifyMiss_Implementation(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
}

void AP4WCharacterPlayer_BLM::ServerRPCApplyDamage_Implementation(const FHitResult& HitResult, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HitResult.GetActor()->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void AP4WCharacterPlayer_BLM::ServerRPCApplyTargetDamage_Implementation(AActor* Actor, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Actor->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

float AP4WCharacterPlayer_BLM::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return ActualDamage;
}

void AP4WCharacterPlayer_BLM::AttackHitCheck()
{
	if (IsLocallyControlled())
	{
		UE_LOG(LogTemp, Log, TEXT("[%s]CurrentDamage: %f"), LOG_NETMODEINFO, CurrentDamage);

		FHitResult OutHitResult;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

		const float AttackRange = Stat->GetTotalStat().AttackRange;
		const float AttackRadius = Stat->GetAttackRadius();
		const float AttackDamage = Stat->GetTotalStat().Attack;
		const FVector Forward = GetActorForwardVector();
		const FVector Start = GetActorLocation() + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
		const FVector End = Start + GetActorForwardVector() * AttackRange;

		bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_P4WACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

		float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		if (!HasAuthority())
		{
			// @Todo: 원거리 공격
			if (HitTarget)
			{
				FDamageEvent DamageEvent;
				float CurrentAttackDamage;
				if (bIsInCombo)
				{
					CurrentAttackDamage = CurrentDamage;
				}
				else
				{
					CurrentAttackDamage = Stat->GetTotalStat().Attack;
				}

				//AP4WCharacterPlayer_PLD* GetHitActorTarget = Cast<AP4WCharacterPlayer_PLD>(HitTarget);
				//if (GetHitActorTarget->bIsSheltron)
				//{
				//	ServerRPCApplyTargetDamage(HitTarget, CurrentAttackDamage * 0.85, DamageEvent, GetController(), this);
				//}
				//else
				//{
					ServerRPCApplyTargetDamage(HitTarget, CurrentAttackDamage, DamageEvent, GetController(), this);
				//}
			}

			if (HitDetected)
			{
				//ServerRPCNotifyHit(OutHitResult, HitCheckTime);
				float CurrentAttackDamage;
				if (bIsInCombo)
				{
					CurrentAttackDamage = CurrentDamage;
				}
				else
				{
					CurrentAttackDamage = Stat->GetTotalStat().Attack;
				}

				FDamageEvent DamageEvent;
				//UE_LOG(LogTemp, Log, TEXT("[%s]Sheltron: %d"), LOG_NETMODEINFO, bIsSheltron);

				//AP4WCharacterPlayer_PLD* GetHitActor = Cast<AP4WCharacterPlayer_PLD>(OutHitResult.GetActor());
				//if (GetHitActor)
				//{
				//	if (GetHitActor->bIsSheltron)
				//	{
				//		ServerRPCApplyDamage(OutHitResult, CurrentAttackDamage * 0.85, DamageEvent, GetController(), this);
				//	}
				//	else
				//	{
						ServerRPCApplyDamage(OutHitResult, CurrentAttackDamage, DamageEvent, GetController(), this);
					//}
				//}
				//OutHitResult.GetActor()->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
				UE_LOG(LogTemp, Log, TEXT("AttackDamage: %f"), CurrentAttackDamage);

				//if (HasAuthority())
				//{

				//}
			}
			else
			{
				ServerRPCNotifyMiss(Start, End, Forward, HitCheckTime);
			}
		}
		else
		{
			if (HitTarget)
			{
				FDamageEvent DamageEvent;
				float CurrentAttackDamage;
				if (bIsInCombo)
				{
					CurrentAttackDamage = CurrentDamage;
				}
				else
				{
					CurrentAttackDamage = Stat->GetTotalStat().Attack;
				}

				//AP4WCharacterPlayer_PLD* GetHitActorTarget = Cast<AP4WCharacterPlayer_PLD>(HitTarget);
				//if (GetHitActorTarget->bIsSheltron)
				//{
				//	HitTarget->TakeDamage(CurrentAttackDamage * 0.85, DamageEvent, GetController(), this);
				//}
				//else
				//{
					HitTarget->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
				//}
			}

			FColor DebugColor = HitDetected ? FColor::Green : FColor::Red;
			//DrawDebugAttackRange(DebugColor, Start, End, Forward);
			if (HitDetected)
			{
				if (HasAuthority())
				{
					float CurrentAttackDamage;
					if (bIsInCombo)
					{
						CurrentAttackDamage = CurrentDamage;
					}
					else
					{
						CurrentAttackDamage = Stat->GetTotalStat().Attack;
					}
					FDamageEvent DamageEvent;
					//UE_LOG(LogTemp, Log, TEXT("Sheltron1: %d"), bIsSheltron);

					//AP4WCharacterPlayer_PLD* GetHitActor = Cast<AP4WCharacterPlayer_PLD>(OutHitResult.GetActor());

					//if (GetHitActor)
					//{
					//	//UE_LOG(LogTemp, Log, TEXT("[%s]Sheltron2: %d"), LOG_NETMODEINFO, GetHitActor->bIsSheltron);
					//	if (GetHitActor->bIsSheltron)
					//	{
					//		OutHitResult.GetActor()->TakeDamage(CurrentAttackDamage * 0.85, DamageEvent, GetController(), this);
					//	}
					//	else
					//	{
							OutHitResult.GetActor()->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
					//	}
					//}
					//UE_LOG(LogTemp, Log, TEXT("AttackDamage: %f"), CurrentAttackDamage);

				}
			}
		}
	}
}

void AP4WCharacterPlayer_BLM::PlayFireAttackAnimation(int32 Time)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FireAttackMontage);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda([&]()
			{
				bIsCasting = false;

				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				AnimInstance->Montage_JumpToSection(TEXT("CastingEnd"), FireAttackMontage);
				//AnimInstance->Montage_Play(FireAttackMontage);

				//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			}
		), Time, false
	);

	FTimerHandle MovementHandle;
	GetWorld()->GetTimerManager().SetTimer(
		MovementHandle,
		FTimerDelegate::CreateLambda([&]()
			{
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			}
		), Time + 0.5f, false
	);
	//AnimInstance->Montage_JumpToSection(ComboNumber, ComboAttackMontage);
}

void AP4WCharacterPlayer_BLM::ServerRPCFireAttack_Implementation(int32 Time)
{
	MulticastRPCFireAttack(Time);
}

void AP4WCharacterPlayer_BLM::MulticastRPCFireAttack_Implementation(int32 Time)
{
	if (!IsLocallyControlled())		// 로컬에서 실행되지 않으면(= 서버가 아니면)
	{
		PlayFireAttackAnimation(Time);
	}
}