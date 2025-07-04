﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P4WCharacterPlayer_PLD.h"
#include "EnhancedInputComponent.h"
#include "Physics/P4WCollision.h"
#include "Interface/P4WAnimationAttackInterface.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EngineUtils.h"
#include "P4W.h"

#include "UI/P4WUserWidget.h"
#include "UI/P4WHpBarWidget.h"
#include "UI/P4WMpBarWidget.h"

#include "Physics/P4WCollision.h"
#include "Engine/DamageEvents.h"
#include "Skill/SkillSystemComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"

#include "GameFramework/Character.h"

#include "Monster/P4WBoss.h"

#include "GameData/P4WGameSingleton.h"
#include "Game/P4WGameInstance.h"

#include "Character/P4WCharacterPlayer_BLM.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AP4WCharacterPlayer_PLD::AP4WCharacterPlayer_PLD()
{
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT(""))

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboAttackMontageRef(TEXT("/Game/Animation/AM_PLDCombo.AM_PLDCombo"));
	if (ComboAttackMontageRef.Object)
	{
		ComboAttackMontage = ComboAttackMontageRef.Object;
	}

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
	
	static ConstructorHelpers::FObjectFinder<UInputAction> CAttackActionRef(TEXT("/Game/Input/IA_CAttack.IA_CAttack"));
	if (CAttackActionRef.Object)
	{
		CAttackAction = CAttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HealUpMontageRef(TEXT("/Game/Animation/AM_HealUp.AM_HealUp"));
	if (HealUpMontageRef.Object)
	{
		HealUpMontage = HealUpMontageRef.Object;
	}

	bReplicates = true;
	bIsInCombo = false;
	bIsSheltron = false;

	bCanPlayCombo1 = true;
	bCanPlayCombo2 = true;
	bCanPlayCombo3 = true;
	bCanPlayHealUp = true;
	bCanPlayProvoke = true;
	bCanPlaySheltron = true;

	// VFX
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SheltronVFXRef(TEXT("/Game/TrackMarker/Niagara/Looping/3Dicon/NS_Icon3D_5.NS_Icon3D_5"));
	if (SheltronVFXRef.Object)
	{
		SheltronVFXSystem = SheltronVFXRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HealUpVFXRef(TEXT("/Game/High_Recovery_VFX/VFX/NS_High_Recovery.NS_High_Recovery"));
	if (HealUpVFXRef.Object)
	{
		HealUpVFXSystem = HealUpVFXRef.Object;
	}
}

void AP4WCharacterPlayer_PLD::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AP4WCharacterPlayer_PLD, bIsInCombo);
	DOREPLIFETIME(AP4WCharacterPlayer_PLD, bIsSheltron);
}

void AP4WCharacterPlayer_PLD::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// Attack Section
	// Auto Attack
	EnhancedInputComponent->BindAction(AutoAttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_PLD::AutoAttack);

	EnhancedInputComponent->BindAction(Combo1AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_PLD::Combo1Attack);
	EnhancedInputComponent->BindAction(Combo2AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_PLD::Combo2Attack);
	EnhancedInputComponent->BindAction(Combo3AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_PLD::Combo3Attack);
	EnhancedInputComponent->BindAction(RAttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_PLD::Sheltron);
	EnhancedInputComponent->BindAction(FAttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_PLD::HealUp);
	EnhancedInputComponent->BindAction(CAttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_PLD::Provoke);
}

void AP4WCharacterPlayer_PLD::AutoAttack(const FInputActionValue& Value)
{
	if (bCanAttack)
	{
		ProcessComboCommand();

		bCanAttack = false;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		//DisableInput(PlayerController);

		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateLambda([&]()
				{
					//EnableInput(Cast<APlayerController>(GetController()));
					//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					bCanAttack = true;
					NotifyComboActionEnd();
				}
			), 1.5f, false
		);
		PlayAutoAttackAnimation();

		ServerRPCAutoAttack();
	}
}

// Cast: Instant, Cooldown time: 2.5s, MP Cost: 0MP, Range: 3y, Radius: 0y
// Damage: 220
void AP4WCharacterPlayer_PLD::Combo1Attack(const FInputActionValue& Value)
{
	if (bCanAttack && bCanPlayCombo1)
	{
		ProcessComboCommand();

		CooldownTime = 2.5f;
		bCanPlayCombo1 = false;

		bCanNextCombo1 = true;
		bCanAttack = false;
		bIsInCombo = true;

		ComboNum = 1;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		//DisableInput(PlayerController);

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateLambda([&]()
				{
					//EnableInput(Cast<APlayerController>(GetController()));
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					bCanAttack = true;
					bIsInCombo = false;
					PrevComboNum = 1;
					NotifyComboActionEnd();
				}
			), 1.0f, false
		);

		FTimerHandle ComboHandle;
		GetWorld()->GetTimerManager().SetTimer(
			ComboHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanNextCombo1 = false;
				}
			), 2.0f, false
		);

		GetWorld()->GetTimerManager().SetTimer(
			CooldownHandle_Combo1,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanPlayCombo1 = true;
				}
			), CooldownTime, false
		);

		// 쿨타임 출력
		UE_LOG(LogTemp, Log, TEXT("CooldownTime: %f/%f"), GetWorld()->GetTimerManager().GetTimerElapsed(CooldownHandle_Combo1), CooldownTime)

		CurrentDamage = 22.0f;

		PlayComboAttackAnimation(ComboNum);

		ServerRPCComboAttack(ComboNum);
	}
}

// Cast: Instant, Cooldown time: 2.5s, MP Cost: 0MP, Range: 3y, Radius: 0y
// Damage: 170 / if Combo Action 1, Combo Potency: 330
void AP4WCharacterPlayer_PLD::Combo2Attack(const FInputActionValue& Value)
{
	if (bCanAttack && bCanPlayCombo2)
	{
		ProcessComboCommand();

		CooldownTime = 2.5f;
		bCanPlayCombo2 = false;

		bCanNextCombo2 = true;
		bCanAttack = false;
		bIsInCombo = true;

		ComboNum = 2;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		//DisableInput(PlayerController);

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateLambda([&]()
				{
					//EnableInput(Cast<APlayerController>(GetController()));
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
					bCanAttack = true;
					bIsInCombo = false;
					PrevComboNum = 2;
					NotifyComboActionEnd();
				}
			), 1.0f, false
		);

		FTimerHandle ComboHandle;
		GetWorld()->GetTimerManager().SetTimer(
			ComboHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanNextCombo2 = false;
				}
			), 2.0f, false
		);

		GetWorld()->GetTimerManager().SetTimer(
			CooldownHandle_Combo2,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanPlayCombo2 = true;
				}
			), CooldownTime, false
		);

		// 쿨타임 출력
		UE_LOG(LogTemp, Log, TEXT("CooldownTime: %f/%f"), GetWorld()->GetTimerManager().GetTimerElapsed(CooldownHandle_Combo2), CooldownTime)

		if (PrevComboNum == 1 && bCanNextCombo1)
		{
			CurrentDamage = 33.0f;
		}
		else
		{
			CurrentDamage = 17.0f;
		}

		PlayComboAttackAnimation(ComboNum);

		ServerRPCComboAttack(ComboNum);
	}
}

// Cast: Instant, Cooldown time: 2.5s, MP Cost: 0MP, Range: 3y, Radius: 0y
// Damage: 100 / if Combo Action 1, Combo Potency: 330
void AP4WCharacterPlayer_PLD::Combo3Attack(const FInputActionValue& Value)
{
	if (bCanAttack && bCanPlayCombo3)
	{
		ProcessComboCommand();

		CooldownTime = 2.5f;
		bCanPlayCombo3 = false;

		bCanAttack = false;
		bIsInCombo = true;

		ComboNum = 3;
		//float PrevDamage = Stat->GetTotalStat().Attack;
		//Stat->SetDamage(220.0f);
		//UE_LOG(LogTemp, Log, TEXT("Combo3Attack | Damage:%f"), Stat->GetTotalStat().Attack);

		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanAttack = true;
					bIsInCombo = false;
					PrevComboNum = 3;
					NotifyComboActionEnd();
				}
			), 1.8f, false
		);

		GetWorld()->GetTimerManager().SetTimer(
			CooldownHandle_Combo3,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanPlayCombo3 = true;
				}
			), CooldownTime, false
		);

		// 쿨타임 출력
		UE_LOG(LogTemp, Log, TEXT("CooldownTime: %f/%f"), GetWorld()->GetTimerManager().GetTimerElapsed(CooldownHandle_Combo3), CooldownTime)

		if (PrevComboNum == 2 && bCanNextCombo2)
		{
			CurrentDamage = 35.0f;
		}
		else
		{
			CurrentDamage = 10.0f;
		}

		PlayComboAttackAnimation(ComboNum);
		ServerRPCComboAttack(ComboNum);

		UE_LOG(LogTemp, Log, TEXT("Combo3Attack | Damage:%f"), Stat->GetTotalStat().Attack);
		//Stat->SetDamage(PrevDamage);

		//UE_LOG(LogTemp, Log, TEXT("[%s] ComboInput ComboNum: %d"), LOG_NETMODEINFO, ComboNum);
	}

}

// Cast: 1.5s, Cooldown time: 2.5s, MP Cost: 2000MP, Range: 30y, Radius: 0y
// Cure Potency: 1000
// FAction
void AP4WCharacterPlayer_PLD::HealUp(const FInputActionValue& Value)
{
	CastingTime = 1.5f;

	// 타겟이 없으면 공격 불가
	if (!HitTarget)
	{
		if (IsLocallyControlled())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("공격할 대상이 없습니다"));
			return;
		}
	}

	AP4WBoss* BossPawn = Cast<AP4WBoss>(HitTarget);
	if (BossPawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("보스에게는 힐을 줄 수 없습니다"));
		return;
	}

	if (Stat->GetCurrentMp() < 2000.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("마나가 부족합니다"));
		return;
	}

	if (bCanAttack && bCanPlayHealUp)
	{
		bIsUsingSkill = true;
		CooldownTime = 2.0f;
		CurrentDamage = 18.0f;
		bCanAttack = false;

		bIsCasting = true;
		bCanPlayHealUp = false;

		Stat->ApplyUseMp(2000.0f);

		PlayHealUpAnimation(CastingTime);

		FTimerHandle HealUpHandle;
		GetWorld()->GetTimerManager().SetTimer(
			HealUpHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanAttack = true;

					// HealUp
					// 대상에게 힐
					AP4WCharacterBase* Target = Cast<AP4WCharacterBase>(HitTarget);
					if (Target)
					{
						if (HasAuthority())
						{
							Target->Stat->HealHp(50.0f);
						}
						else
						{
							ServerRPCHealUp(Target);
						}
						UE_LOG(LogTemp, Log, TEXT("[%s] Target HP: %f"), LOG_NETMODEINFO, Target->Stat->GetCurrentHp());
					}
				}
			), CastingTime, false
		);

		FTimerHandle HealUpVFXHandle;
		GetWorld()->GetTimerManager().SetTimer(
			HealUpVFXHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					HealUpVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HealUpVFXSystem, HitTarget->GetActorLocation());
					if (HealUpVFXComponent)
					{
						if (HasAuthority())
						{
							HealUpVFXComponent->Activate();
							MulticastRPCHealUpVFX(HealUpVFXSystem, HitTarget);
						}
						else
						{
							HealUpVFXComponent->Activate();
							ServerRPCHealUpVFX(HealUpVFXSystem, HitTarget);
						}
					}
				}
			), CastingTime - 0.5f, false
		);

		//PlayFireAttackAnimation();
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		GetWorld()->GetTimerManager().SetTimer(
			CooldownHandle_HealUp,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanPlayHealUp = true;
				}
			), CooldownTime, false
		);

		/*
		//UE_LOG(LogTemp, Log, TEXT("bIsCasting: %d, bIsAnyKeyPressed: %d"), bIsCasting, bIsAnyKeyPressed);

		//if (bIsCasting && bIsAnyKeyPressed)
		//{
		//	UE_LOG(LogTemp, Log, TEXT("bIsCasting: %d, bIsAnyKeyPressed: %d"), bIsCasting, bIsAnyKeyPressed);
		//	//AnimInstance->StopAllMontages(0.5f);
		//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		//	AnimInstance->Montage_Stop(0.0f, HealUpMontage);

		//	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		//}
		*/

		ServerRPCHealUpAnimation(CastingTime);
	}
}

// Cast: Instant, Cooldown time: 5s, MP Cost: 0MP, Range: 0y, Radius: 0y
// Reduces damage taken by 15%
// Duration: 6s, Oath Gauge Cost: 50
// RAction
void AP4WCharacterPlayer_PLD::Sheltron(const FInputActionValue& Value)
{
	if (bCanPlaySheltron)
	{
		SheltronVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			SheltronVFXSystem, 
			GetMesh(), 
			NAME_None, 
			FVector(0.0f, 0.0f, 80.0f), 
			FRotator::ZeroRotator, 
			EAttachLocation::KeepRelativeOffset,
			true
		);

		if (SheltronVFXComponent)
		{
			SheltronVFXComponent->Activate();
		}

		bIsSheltron = true;
		bCanPlaySheltron = false;
		CooldownTime = 10.0f;

		GetWorld()->GetTimerManager().SetTimer(
			SheltronHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					bIsSheltron = false;
					bCanPlaySheltron = true;
				}
			), 6.0f, false
		);

		FTimerHandle SheltronVFXHandle;
		GetWorld()->GetTimerManager().SetTimer(
			SheltronVFXHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					if (SheltronVFXComponent)
					{
						SheltronVFXComponent->Deactivate();
					}
				}
			), 0.5f, false
		);
	}

	else
	{
		float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(SheltronHandle);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("Sheltron 쿨타임 도는 중 ... (%f / %f)"), ElapsedTime, CooldownTime));
	}
}

// Cast: Instant, Cooldown time: 30s, MP Cost: 0MP, Range: 25y, Radius: 0y
// placing yourself at the top of a target's enmity list while gaining additional enmity
// CAction
void AP4WCharacterPlayer_PLD::Provoke(const FInputActionValue& Value)
{
	if (bCanPlayProvoke)
	{
		bCanPlayProvoke = false;
		CooldownTime = 30.0f;

		Stat->UpdateEnmity(300.0f);
		
		//SetMaxEnmity(Stat->GetCurrentEnmity());
		//SetBossMaxEnmity
		//BossPtr->SetBossMaxEnmity(Stat->GetCurrentEnmity());

		//UP4WGameSingleton::Get().MaxEnmity = Stat->GetCurrentEnmity();
		UP4WGameInstance* GameInstance = Cast<UP4WGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			if (HasAuthority())
			{
				GameInstance->MaxEnmity = Stat->GetCurrentEnmity();
			}
			else
			{
				ServerRPCMaxEnmity(this, Stat->GetCurrentEnmity());
			}
		}

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Current Enmity: %f"), GameInstance->MaxEnmity));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Current Enmity2: %f"), Stat->GetCurrentEnmity()));

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanPlayProvoke = true;
				}
			), CooldownTime, false
		);
	}
}

bool AP4WCharacterPlayer_PLD::ServerRPCNotifyHit_Validate(const FHitResult& HitResult, float HitCheckTime)
{
	return true;
}

void AP4WCharacterPlayer_PLD::ServerRPCNotifyHit_Implementation(const FHitResult& HitResult, float HitCheckTime)
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
					if (bIsSheltron)
					{
						Stat->SetDamage(CurrentDamage * 0.85);
					}
					else
					{
						Stat->SetDamage(CurrentDamage);
					}
				}
				else
				{
					//UE_LOG(LogTemp, Log, TEXT("[%s]CurrentDamage In If: %f"), LOG_NETMODEINFO, CurrentDamage);
					//AttackDamage = Stat->GetTotalStat().Attack;
					//Stat->SetDamage(30.0f);
				}
				FDamageEvent DamageEvent;
				if (bIsSheltron)
				{
					HitActor->TakeDamage(Stat->GetTotalStat().Attack * 0.85, DamageEvent, GetController(), this);
				}
				else
				{
					HitActor->TakeDamage(Stat->GetTotalStat().Attack, DamageEvent, GetController(), this);
				}
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

bool AP4WCharacterPlayer_PLD::ServerRPCNotifyMiss_Validate(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	return true;
}

void AP4WCharacterPlayer_PLD::ServerRPCNotifyMiss_Implementation(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
}

void AP4WCharacterPlayer_PLD::ServerRPCApplyDamage_Implementation(const FHitResult& HitResult, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	AP4WCharacterPlayer_PLD* PLDPawn = Cast<AP4WCharacterPlayer_PLD>(HitResult.GetActor());
	if (PLDPawn && PLDPawn->bIsSheltron)
	{
		HitResult.GetActor()->TakeDamage(Damage * 0.85, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		HitResult.GetActor()->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	}
}

void AP4WCharacterPlayer_PLD::ServerRPCApplyTargetDamage_Implementation(AActor* Actor, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	AP4WCharacterPlayer_PLD* PLDPawn = Cast<AP4WCharacterPlayer_PLD>(Actor);
	if (PLDPawn && PLDPawn->bIsSheltron)
	{
		Actor->TakeDamage(Damage * 0.85, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		Actor->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
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
				
				// 플레이어 -> 플레이어 공격 막음
				AP4WCharacterPlayer_PLD* PLDPawn = Cast<AP4WCharacterPlayer_PLD>(HitTarget);
				if (PLDPawn)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("플레이어는 공격할 수 없습니다"));
					return;
				}

				AP4WCharacterPlayer_BLM* BLMPawn = Cast<AP4WCharacterPlayer_BLM>(HitTarget);
				if (BLMPawn)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("플레이어는 공격할 수 없습니다"));
					return;
				}

				//AP4WCharacterPlayer_PLD* GetHitActorTarget = Cast<AP4WCharacterPlayer_PLD>(HitTarget);
				ServerRPCApplyTargetDamage(HitTarget, CurrentAttackDamage, DamageEvent, GetController(), this);

				//if (GetHitActorTarget->bIsSheltron)
				//{
				//	ServerRPCApplyTargetDamage(HitTarget, CurrentAttackDamage * 0.85, DamageEvent, GetController(), this);
				//}
				//else
				//{
				//	ServerRPCApplyTargetDamage(HitTarget, CurrentAttackDamage, DamageEvent, GetController(), this);
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

				// 플레이어 -> 플레이어 공격 막음
				AP4WCharacterPlayer_PLD* PLDPawn = Cast<AP4WCharacterPlayer_PLD>(OutHitResult.GetActor());
				if (PLDPawn)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("플레이어는 공격할 수 없습니다"));
					return;
				}

				AP4WCharacterPlayer_BLM* BLMPawn = Cast<AP4WCharacterPlayer_BLM>(OutHitResult.GetActor());
				if (BLMPawn)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("플레이어는 공격할 수 없습니다"));
					return;
				}

				FDamageEvent DamageEvent;
				UE_LOG(LogTemp, Log, TEXT("[%s]Sheltron: %d"), LOG_NETMODEINFO, bIsSheltron);

				ServerRPCApplyDamage(OutHitResult, CurrentAttackDamage, DamageEvent, GetController(), this);
				
				//AP4WCharacterPlayer_PLD* GetHitActor = Cast<AP4WCharacterPlayer_PLD>(OutHitResult.GetActor());
				//if (GetHitActor)
				//{
				//	if (GetHitActor->bIsSheltron)
				//	{
				//		ServerRPCApplyDamage(OutHitResult, CurrentAttackDamage * 0.85, DamageEvent, GetController(), this);
				//	}
				//	else
				//	{
				//		ServerRPCApplyDamage(OutHitResult, CurrentAttackDamage, DamageEvent, GetController(), this);
				//	}
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
				/*
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

				// 플레이어 -> 플레이어 공격 막음
				AP4WCharacterPlayer_PLD* PLDPawn = Cast<AP4WCharacterPlayer_PLD>(HitTarget);
				if (PLDPawn)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("플레이어는 공격할 수 없습니다"));
					return;
				}

				AP4WCharacterPlayer_BLM* BLMPawn = Cast<AP4WCharacterPlayer_BLM>(HitTarget);
				if (BLMPawn)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("플레이어는 공격할 수 없습니다"));
					return;
				}


				//AP4WCharacterPlayer_PLD* PLDPawn = Cast<AP4WCharacterPlayer_PLD>(HitTarget);
				//if (PLDPawn && PLDPawn->bIsSheltron)
				//{
				//	HitTarget->TakeDamage(CurrentAttackDamage * 0.85, DamageEvent, GetController(), this);
				//}
				//else
				//{
				//	HitTarget->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
				//}

				HitTarget->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
				*/

				//AP4WCharacterPlayer_PLD* GetHitActorTarget = Cast<AP4WCharacterPlayer_PLD>(HitTarget);
				//if (GetHitActorTarget->bIsSheltron)
				//{
				//	HitTarget->TakeDamage(CurrentAttackDamage * 0.85, DamageEvent, GetController(), this);
				//}
				//else
				//{
				//	HitTarget->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
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

					// 플레이어 -> 플레이어 공격 막음
					AP4WCharacterPlayer_PLD* PLDPawn = Cast<AP4WCharacterPlayer_PLD>(OutHitResult.GetActor());
					if (PLDPawn)
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("플레이어는 공격할 수 없습니다"));
						return;
					}

					AP4WCharacterPlayer_BLM* BLMPawn = Cast<AP4WCharacterPlayer_BLM>(OutHitResult.GetActor());
					if (BLMPawn)
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("플레이어는 공격할 수 없습니다"));
						return;
					}

					FDamageEvent DamageEvent; 
					//AP4WCharacterPlayer_PLD* PLDPawn = Cast<AP4WCharacterPlayer_PLD>(OutHitResult.GetActor());
					//if (PLDPawn && PLDPawn->bIsSheltron)
					//{
					//	PLDPawn->TakeDamage(CurrentAttackDamage * 0.85, DamageEvent, GetController(), this);
					//}
					//else
					//{
					//	PLDPawn->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
					//}

					OutHitResult.GetActor()->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);

					//AP4WCharacterPlayer_PLD* GetHitActor = Cast<AP4WCharacterPlayer_PLD>(OutHitResult.GetActor());

					//if (GetHitActor)
					//{
					//	UE_LOG(LogTemp, Log, TEXT("[%s]Sheltron2: %d"), LOG_NETMODEINFO, GetHitActor->bIsSheltron);
					//	if (GetHitActor->bIsSheltron)
					//	{
					//		OutHitResult.GetActor()->TakeDamage(CurrentAttackDamage * 0.85, DamageEvent, GetController(), this);
					//	}
					//	else
					//	{
					//		OutHitResult.GetActor()->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
					//	}
					//}
					UE_LOG(LogTemp, Log, TEXT("AttackDamage: %f"), CurrentAttackDamage);
				}
			}
		}
	}
}

void AP4WCharacterPlayer_PLD::SpellHitCheck()
{
}

void AP4WCharacterPlayer_PLD::SpellHitCheckDoT()
{
}

float AP4WCharacterPlayer_PLD::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return ActualDamage;
}

void AP4WCharacterPlayer_PLD::DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
#if ENABLE_DRAW_DEBUG

	const float AttackRange = Stat->GetTotalStat().AttackRange;
	const float AttackRadius = Stat->GetAttackRadius();
	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);

#endif
}

void AP4WCharacterPlayer_PLD::PlayHealUpAnimation(int32 Time)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(HealUpMontage);
}

void AP4WCharacterPlayer_PLD::ServerRPCHealUpVFX_Implementation(UNiagaraSystem* NS, AActor* HealActor)
{
	MulticastRPCHealUpVFX(NS, HealActor);
}

void AP4WCharacterPlayer_PLD::MulticastRPCHealUpVFX_Implementation(UNiagaraSystem* NS, AActor* HealActor)
{
	HealUpVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS, HealActor->GetActorLocation());
	HealUpVFXComponent->Activate();
}

void AP4WCharacterPlayer_PLD::ServerRPCMaxEnmity_Implementation(AP4WCharacterBase* Target, float Enmity)
{
	UP4WGameInstance* GameInstance = Cast<UP4WGameInstance>(Target->GetGameInstance());
	if (GameInstance)
	{
		GameInstance->MaxEnmity = Enmity;
		UE_LOG(LogTemp, Log, TEXT("CurrentEnmity: %f"), Target->Stat->GetCurrentEnmity());
		UE_LOG(LogTemp, Log, TEXT("CurrentEnmity2: %f"), GameInstance->MaxEnmity);
	}
}

void AP4WCharacterPlayer_PLD::ServerRPCHealUp_Implementation(AP4WCharacterBase* Target)
{
	Target->Stat->HealHp(50.0f);
}

//void AP4WCharacterPlayer_PLD::MulticastRPCHealUp_Implementation()
//{
//
//}

void AP4WCharacterPlayer_PLD::ServerRPCHealUpAnimation_Implementation(int32 Time)
{
	MulticastRPCHealUpAnimation(Time);
}

void AP4WCharacterPlayer_PLD::MulticastRPCHealUpAnimation_Implementation(int32 Time)
{
	if (!IsLocallyControlled())		// 로컬에서 실행되지 않으면(= 서버가 아니면)
	{
		PlayHealUpAnimation(Time);
	}
}

/*
void AP4WCharacterPlayer_PLD::ComboActionBegin()
{
	//// 콤보 상태를 1로 설정.
	//CurrentCombo = 1;

	//// 이동 비활성화.
	////GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	//// 몽타주 재생.
	//// 애님 인스턴스는 스켈레탈 메시가 가지고 있음
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//if (AnimInstance)
	//{
	//	//const float AttackSpeedRate = 1.0f;
	//	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	//	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	//	// 몽타주 재생이 시작되면, 재생이 종료될 때 호출되는 델리게이트에 등록.
	//	FOnMontageEnded EndDelegate;
	//	EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);
	//	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);	// 몽타주가 끝나면 ComboActionEnd라는 함수가 호출됨

	//	// 콤보 확인을 위한 타이머 설정.
	//	ComboTimerHandle.Invalidate();
	//	SetComboCheckTimer();
	//}
	
}

void AP4WCharacterPlayer_PLD::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	//// 유효성 검사.
	//ensure(CurrentCombo != 0);

	//// 콤보 초기화.
	//CurrentCombo = 0;

	//// 캐릭터 무브먼트 컴포넌트 모드 복구.
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//// 공격이 끝나면 NotifyComboActionEnd 함수 호출.
	//NotifyComboActionEnd();
}

//void AP4WCharacterPlayer_PLD::SetComboCheckTimer()
//{
//	int32 ComboIndex = ComboNum - 1;
//	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));
//
//	const float AttackSpeedRate = 1.0f;
//	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
//	if (ComboEffectiveTime > 0.0f)
//	{
//		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AP4WCharacterPlayer_PLD::ComboCheck, ComboEffectiveTime, false);
//	}
//}

//bool AP4WCharacterPlayer_PLD::ComboCheck()
//{
//	FTimerHandle ComboHandle;
//	GetWorld()->GetTimerManager().SetTimer(
//		ComboHandle,
//		FTimerDelegate::CreateLambda([&]()
//			{
//				return false;
//			}
//		), 2.0f, false
//	);
//
//	return true;
//}

//void AP4WCharacterPlayer_PLD::ComboCheck()
//{
//	ComboTimerHandle.Invalidate(); 
//	if (HasNextComboCommand)
//	{
//		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//
//		ComboNum = FMath::Clamp(ComboNum + 1, 1, ComboActionData->MaxComboCount);
//		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, ComboNum);
//		//AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
//		SetComboCheckTimer();
//		HasNextComboCommand = false;
//	}
//}
*/