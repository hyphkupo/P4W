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

#include "Character/P4WCharacterPlayer_PLD.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Skill/MagicProjectile.h"

#include "UI/P4WWidgetComponent.h"
#include "UI/P4WCooltimeWidget.h"
#include "UI/P4WHUDWidget.h"
#include "Blueprint/UserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Player/P4WPlayerController.h"

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ThunderAttackMontageRef(TEXT("/Game/Animation/AM_ThunderAttack.AM_ThunderAttack"));
	if (ThunderAttackMontageRef.Object)
	{
		ThunderAttackMontage = ThunderAttackMontageRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireBallAttackMontageRef(TEXT("/Game/Animation/AM_FireBallAttack.AM_FireBallAttack"));
	if (FireBallAttackMontageRef.Object)
	{
		FireBallAttackMontage = FireBallAttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ManafontMontageRef(TEXT("/Game/Animation/AM_Manafont.AM_Manafont"));
	if (ManafontMontageRef.Object)
	{
		ManafontMontage = ManafontMontageRef.Object;
	}

	bReplicates = true;
	bCanAttack = true;
	bIsAnyKeyPressed = false;
	bIsCasting = false;
	CastingTime = 0.0f;

	CurrentDamage = Stat->GetTotalStat().Attack;

	bCanPlayBlizzardAttack = true;
	bCanPlayFireAttack = true;
	bCanPlayThunderAttack = true;
	bCanPlayFireBallAttack = true;
	bCanPlayManafont = true;

	// VFX
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ManafontVFXRef(TEXT("/Game/High_Recovery_VFX/VFX/NS_High_Recovery_Owner.NS_High_Recovery_Owner"));
	if (ManafontVFXRef.Object)
	{
		ManafontVFXSystem = ManafontVFXRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BlizzardTrailEffectRef(TEXT("/Game/Ultimate_Magic_Bundle/MagicProjectiles/Niagara/Ice/NS_Ice_Trail.NS_Ice_Trail"));
	if (BlizzardTrailEffectRef.Object)
	{
		BlizzardTrailEffect = BlizzardTrailEffectRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BlizzardImpactEffectRef(TEXT("/Game/Ultimate_Magic_Bundle/AEO_Abilities/MagicAEO/Niagara/Ice/NS_IceAEO.NS_IceAEO"));
	if (BlizzardImpactEffectRef.Object)
	{
		BlizzardImpactEffect = BlizzardImpactEffectRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireImpactEffectRef(TEXT("/Game/Ultimate_Magic_Bundle/AEO_Abilities/MagicAEO/Niagara/Ice/NS_IceAEO.NS_IceAEO"));
	if (FireImpactEffectRef.Object)
	{
		FireImpactEffect = FireImpactEffectRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ThunderImpactEffectRef(TEXT("/Game/Ultimate_Magic_Bundle/AEO_Abilities/MagicAEO/Niagara/Lightning/NS_LightningAEO.NS_LightningAEO"));
	if (ThunderImpactEffectRef.Object)
	{
		ThunderImpactEffect = ThunderImpactEffectRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireBallImpactEffectRef(TEXT("/Game/Ultimate_Magic_Bundle/AEO_Abilities/MagicAEO/Niagara/Fire/NS_FireAEO.NS_FireAEO"));
	if (FireBallImpactEffectRef.Object)
	{
		FireBallImpactEffect = FireBallImpactEffectRef.Object;
	}

	// Widget Component

}

void AP4WCharacterPlayer_BLM::BeginPlay()
{
	Super::BeginPlay();

	
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
		bIsUsingAutoAttack = true;
		ProcessComboCommand();

		bCanAttack = false;

		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanAttack = true;
					bIsUsingAutoAttack = false;
					NotifyComboActionEnd();
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

	if (Stat->GetCurrentMp() < 400.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("마나가 부족합니다"));
		return;
	}

	if (bCanAttack && bCanPlayBlizzardAttack)
	{
		//for (UUserWidget* Widget : UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, UP4WHUDWidget::StaticClass(), false))
		//{
		//	UP4WHUDWidget* HUD = Cast<UP4WHUDWidget>(Widget);
		//	if (HUD && HUD->CooltimeBar)
		//	{
		//		HUD->CooltimeBar->StartCooltime(3.0f);
		//		break;
		//	}
		//}


		//APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		//UP4WHUDWidget* HUD = Cast<UP4WHUDWidget>(PC->GetHUD());

		//if (HUD)
		//{
		//	HUD->CooltimeBar->StartCooltime(3.0f);
		//}


		//CooltimeBar->StartCooltime(3.0f);
		//UP4WCooltimeWidget* CooltimeWidget = Cast<UP4WCooltimeWidget>(CooltimeBar);
		//if (CooltimeWidget)
		//{
		//	CooltimeWidget->StartCooltime(3.0f);
		//}
		ProcessComboCommand();

		bIsUsingSkill = true;
		CooldownTime = 2.5f;
		CurrentDamage = 15.0f;
		bCanAttack = false;

		bIsCasting = true;
		bCanPlayBlizzardAttack = false;

		Stat->ApplyUseMp(400.0f);

		PlayFireAttackAnimation(CastingTime);

		FVector Start = this->GetActorLocation();
		FVector Target = HitTarget->GetActorLocation();
		FVector Dir = (Target - Start).GetSafeNormal();

		Spell = GetWorld()->SpawnActor<AMagicProjectile>(
			MagicProjectileClass,
			Start,
			Dir.Rotation()
		);

		//if (CooltimeWidget)
		//{
		//	CooltimeWidget->StartCooltime(CooldownTime);
		//}

		//Spell->LaunchProjectile(Dir, HitTarget);

		GetWorld()->GetTimerManager().SetTimer(
			CooldownHandle_BlizzardAttack,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanPlayBlizzardAttack = true;

					ACharacter* Char = Cast<ACharacter>(HitTarget);

					BlizzardImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
						BlizzardImpactEffect,
						Char->GetMesh(),
						NAME_None,
						FVector(0.0f, 0.0f, 0.0f),
						FRotator::ZeroRotator,
						EAttachLocation::KeepRelativeOffset,
						true
					);

					if (BlizzardImpactEffectComponent)
					{
						if (HasAuthority())
						{
							MulticastRPCBlizzardVFX(BlizzardImpactEffect, HitTarget);
						}
						else
						{
							ServerRPCBlizzardVFX(BlizzardImpactEffect, HitTarget);
						}
					}
				}
			), CooldownTime, false
		);

		FTimerHandle BlizzardVFXHandle;
		GetWorld()->GetTimerManager().SetTimer(
			BlizzardVFXHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					if (BlizzardImpactEffectComponent)
					{
						BlizzardImpactEffectComponent->Deactivate();
					}
				}
			), CooldownTime + 2.0f, false
		);

		UE_LOG(LogTemp, Log, TEXT("bIsCasting: %d, bIsAnyKeyPressed: %d"), bIsCasting, bIsAnyKeyPressed);

		//if (bIsCasting && bIsAnyKeyPressed)
		//{
		//	UE_LOG(LogTemp, Log, TEXT("bIsCasting: %d, bIsAnyKeyPressed: %d"), bIsCasting, bIsAnyKeyPressed);
		//	//AnimInstance->StopAllMontages(0.5f);
		//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		//	AnimInstance->Montage_Stop(0.0f, FireAttackMontage);		// 수정

		//	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		//}

		ServerRPCFireAttack(CastingTime);
	}
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

	if (Stat->GetCurrentMp() < 800.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("마나가 부족합니다"));
		return;
	}

	if (bCanAttack && bCanPlayFireAttack)
	{
		ProcessComboCommand();

		bIsUsingSkill = true;
		CooldownTime = 2.5f;
		CurrentDamage = 18.0f;
		bCanAttack = false;

		bIsCasting = true;
		bCanPlayFireAttack = false;

		Stat->ApplyUseMp(800.0f);

		

		//PlayFireAttackAnimation();
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		PlayFireAttackAnimation(CastingTime);

		FTimerHandle FireVFXHandle;
		GetWorld()->GetTimerManager().SetTimer(
			FireVFXHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					if (HasAuthority())
					{
						//Projectile->LaunchToTarget(HitTarget);

						MulticastRPCFireVFX(ThunderImpactEffect, HitTarget);
					}
					else
					{
						ServerRPCFireVFX(ThunderImpactEffect, HitTarget);
					}
					
				}
			), CastingTime, false
		);

		GetWorld()->GetTimerManager().SetTimer(
			CooldownHandle_FireAttack,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanPlayFireAttack = true;
				}
			), CooldownTime, false
		);

		UE_LOG(LogTemp, Log, TEXT("bIsCasting: %d, bIsAnyKeyPressed: %d"), bIsCasting, bIsAnyKeyPressed);

		//if (bIsCasting && bIsAnyKeyPressed)
		//{
		//	UE_LOG(LogTemp, Log, TEXT("bIsCasting: %d, bIsAnyKeyPressed: %d"), bIsCasting, bIsAnyKeyPressed);
		//	//AnimInstance->StopAllMontages(0.5f);
		//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		//	AnimInstance->Montage_Stop(0.0f, FireAttackMontage);

		//	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		//}

		ServerRPCFireAttack(CastingTime);
	}
}

// Cast: Instant, Cooldown time: 2.5s, MP Cost: 0MP, Range: 25y, Radius: 0y
// Damage: 100, Potency: 45, Duration: 24s
void AP4WCharacterPlayer_BLM::ThunderAttack(const FInputActionValue& Value)
{
	CastingTime = 0.0f;

	// 타겟이 없으면 공격 불가
	if (!HitTarget)
	{
		if (IsLocallyControlled())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("공격할 대상이 없습니다"));
			return;
		}
	}

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

	if (bCanAttack && bCanPlayThunderAttack)
	{
		ACharacter* Char = Cast<ACharacter>(HitTarget);

		ThunderImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ThunderImpactEffect,
			Char->GetMesh(),
			NAME_None,
			FVector(0.0f, 0.0f, 0.0f),
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);

		//ThunderImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ThunderImpactEffect, HitTarget->GetActorLocation());
		if (ThunderImpactEffectComponent)
		{
			if (HasAuthority())
			{
				//ThunderImpactEffectComponent->Activate();
				MulticastRPCThunderVFX(ThunderImpactEffect, HitTarget);
			}
			else
			{
				//ThunderImpactEffectComponent->Activate();
				ServerRPCThunderVFX(ThunderImpactEffect, HitTarget);
			}
		}

		ProcessComboCommand();

		CooldownTime = 2.5f;
		CurrentDamage = 10.0f;

		DotTime = 24.0f;
		Potency = 4.5f;

		bIsCasting = true;
		bCanPlayThunderAttack = false;

		bIsThunder = true;

		PlayThunderAttackAnimation(CastingTime);

		GetWorld()->GetTimerManager().SetTimer(
			CooldownHandle_ThunderAttack,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanPlayThunderAttack = true;
					if (ThunderImpactEffectComponent)
					{
						ThunderImpactEffectComponent->Deactivate();
					}
				}
			), CooldownTime, false
		);

		if (bIsThunder)
		{
			GetWorld()->GetTimerManager().ClearTimer(DotManagerHandle);
			GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
		}

		UE_LOG(LogTemp, Log, TEXT("bIsCasting: %d, bIsAnyKeyPressed: %d"), bIsCasting, bIsAnyKeyPressed);

		if (bIsCasting && bIsAnyKeyPressed)
		{
			UE_LOG(LogTemp, Log, TEXT("bIsCasting: %d, bIsAnyKeyPressed: %d"), bIsCasting, bIsAnyKeyPressed);
			//AnimInstance->StopAllMontages(0.5f);
			//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			//AnimInstance->Montage_Stop(0.0f, FireAttackMontage);

			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}

		ServerRPCThunderAttack(CastingTime, DotTime, Potency);
	}
}

// RAttackAction
// Cast: 3.5s, Cooldown time: 2.5s, MP Cost: 800MP, Range: 25y, Radius: 0y
// Damage: 300
void AP4WCharacterPlayer_BLM::FireBallAttack(const FInputActionValue& Value)
{
	CastingTime = 3.5f;

	// 타겟이 없으면 공격 불가
	if (!HitTarget)
	{
		if (IsLocallyControlled())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("공격할 대상이 없습니다"));
			return;
		}
	}

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

	if (Stat->GetCurrentMp() < 800.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("마나가 부족합니다"));
		return;
	}

	if (bCanAttack)
	{
		ProcessComboCommand();

		bIsUsingSkill = true;
		CooldownTime = 2.5f;
		CurrentDamage = 30.0f;
		bCanAttack = false;

		bIsCasting = true;
		bCanPlayFireBallAttack = false;

		Stat->ApplyUseMp(800.0f);

		PlayFireBallAttackAnimation(CastingTime);

		GetWorld()->GetTimerManager().SetTimer(
			CooldownHandle_FireBallAttack,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanPlayFireBallAttack = true;
				}
			), CooldownTime, false
		);

		FTimerHandle ImpactHandle;
		GetWorld()->GetTimerManager().SetTimer(
			CooldownHandle_FireBallAttack,
			FTimerDelegate::CreateLambda([&]()
				{
					ACharacter* Char = Cast<ACharacter>(HitTarget);

					FireBallImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
						FireBallImpactEffect,
						Char->GetMesh(),
						NAME_None,
						FVector::ZeroVector,
						FRotator::ZeroRotator,
						EAttachLocation::SnapToTargetIncludingScale,
						true
					);

					//FireBallImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireBallImpactEffect, HitTarget->GetActorLocation());
					if (FireBallImpactEffectComponent)
					{
						if (HasAuthority())
						{
							//FireBallImpactEffectComponent->Activate();
							MulticastRPCFireBallVFX(FireBallImpactEffect, HitTarget);
						}
						else
						{
							//FireBallImpactEffectComponent->Activate();
							ServerRPCFireBallVFX(FireBallImpactEffect, HitTarget);
						}
					}
				}
			), CooldownTime + 1.0f, false
		);
		

		FTimerHandle FireBallVFXHandle;
		GetWorld()->GetTimerManager().SetTimer(
			FireBallVFXHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					if (FireBallImpactEffectComponent)
					{
						FireBallImpactEffectComponent->Deactivate();
					}
				}
			), CooldownTime + 2.0f, false
		);

		UE_LOG(LogTemp, Log, TEXT("bIsCasting: %d, bIsAnyKeyPressed: %d"), bIsCasting, bIsAnyKeyPressed);

		//if (bIsCasting && bIsAnyKeyPressed)
		//{
		//	UE_LOG(LogTemp, Log, TEXT("bIsCasting: %d, bIsAnyKeyPressed: %d"), bIsCasting, bIsAnyKeyPressed);
		//	//AnimInstance->StopAllMontages(0.5f);
		//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		//	AnimInstance->Montage_Stop(0.0f, FireBallAttackMontage);

		//	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		//}

		ServerRPCFireBallAttack(CastingTime);
	}
}

// FAttackAction
// Cast: Instant, Cooldown time: 120s, MP Cost: 0MP, Range: 0y, Radius: 0y
// Fully restores MP.
void AP4WCharacterPlayer_BLM::Manafont(const FInputActionValue& Value)
{
	CastingTime = 0.0f;
	CooldownTime = 120.0f;

	if (bCanAttack && bCanPlayManafont)
	{
		ManafontVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ManafontVFXSystem,
			GetMesh(),
			NAME_None,
			FVector(0.0f, 0.0f, 60.0f),
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);

		if (ManafontVFXComponent)
		{
			ManafontVFXComponent->Activate();
		}

		//if (CooltimeBar)
		//{
		//	CooltimeBar->UpdateCooltimeBar();
		//}

		APlayerController* PC = Cast<APlayerController>(GetController());
		if (!PC) return;

		AP4WPlayerController* MyPC = Cast<AP4WPlayerController>(PC);
		if (!MyPC || !MyPC->HUDWidget) return;

		UP4WCooltimeWidget* CooltimeWidget = MyPC->HUDWidget->CooltimeBar;

		if (CooltimeWidget)
		{
			CooltimeWidget->StartCooltime(3.0f);
		}


		FTimerHandle ManafontVFXHandle;
		GetWorld()->GetTimerManager().SetTimer(
			ManafontVFXHandle,
			FTimerDelegate::CreateLambda([&]()
				{
					if (ManafontVFXComponent)
					{
						ManafontVFXComponent->Deactivate();
					}
				}
			), 0.5f, false
		);

		Stat->SetMpMax();

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		
		bIsUsingSkill = true;

		bIsCasting = true;
		bCanPlayManafont = false;

		PlayManafontAnimation(CastingTime);

		// ManafontMontage
		//GetMesh()->PlayAnimation(ManafontAnimation, false);
		
		GetWorld()->GetTimerManager().SetTimer(
			CooldownHandle_Manafont,
			FTimerDelegate::CreateLambda([&]()
				{
					bCanPlayManafont = true;
				}
			), CooldownTime, false
		);

		ServerRPCManafont(CastingTime);
	}

	else
	{
		float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(CooldownHandle_Manafont);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::Printf(TEXT("마나샘 쿨타임 도는 중 ... (%f / %f)"), ElapsedTime, CooldownTime));
	}
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
		//InHUDWidget->CooltimeBar->UpdateCooltimeBar();

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
				if (bIsUsingAutoAttack)
				{
					return;
				}
				FDamageEvent DamageEvent;
				float CurrentAttackDamage;
				if (bIsCasting)
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
				if (bIsCasting)
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
				if (bIsUsingAutoAttack)
				{
					return;
				}
				FDamageEvent DamageEvent;
				float CurrentAttackDamage;
				if (bIsCasting)
				{
					CurrentAttackDamage = CurrentDamage;
				}
				else
				{
					CurrentAttackDamage = Stat->GetTotalStat().Attack;
					//CurrentAttackDamage = CurrentDamage;

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
					if (bIsCasting)
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

void AP4WCharacterPlayer_BLM::SpellHitCheck()
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
				if (bIsUsingSkill)
				{
					CurrentAttackDamage = CurrentDamage;
				}
				else
				{
					CurrentAttackDamage = Stat->GetTotalStat().Attack;
				}

				ServerRPCApplyTargetDamage(HitTarget, CurrentAttackDamage, DamageEvent, GetController(), this);
			}

			if (HitDetected)
			{
				float CurrentAttackDamage;
				
				if (bIsUsingSkill)
				{
					CurrentAttackDamage = CurrentDamage;
				}
				else
				{
					CurrentAttackDamage = Stat->GetTotalStat().Attack;
				}

				FDamageEvent DamageEvent;

				ServerRPCApplyDamage(OutHitResult, CurrentAttackDamage, DamageEvent, GetController(), this);

				UE_LOG(LogTemp, Log, TEXT("AttackDamage: %f"), CurrentAttackDamage);
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
				if (bIsUsingSkill)
				{
					CurrentAttackDamage = CurrentDamage;
				}
				else
				{
					CurrentAttackDamage = Stat->GetTotalStat().Attack;
				}

				HitTarget->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
			}

			FColor DebugColor = HitDetected ? FColor::Green : FColor::Red;
			if (HitDetected)
			{
				if (HasAuthority())
				{
					float CurrentAttackDamage;
					if (bIsUsingSkill)
					{
						CurrentAttackDamage = CurrentDamage;
					}
					else
					{
						CurrentAttackDamage = Stat->GetTotalStat().Attack;
					}
					FDamageEvent DamageEvent;
					OutHitResult.GetActor()->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
				}
			}
		}
	}
}

void AP4WCharacterPlayer_BLM::SpellHitCheckDoT()
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
				CurrentAttackDamage = CurrentDamage;
				HitTarget->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);

				ServerRPCApplyTargetDamage(HitTarget, CurrentAttackDamage, DamageEvent, GetController(), this);

				//FTimerHandle DoTHandle;
				//uint32 Duration = DotTime;
				
				//FTimerHandle DurationHandle;
				//FTimerHandle DelayHandle;

				// 타겟 해제 방지용
				DotHitTarget = HitTarget;

				//RepeatingFunction();


				//GetWorld()->GetTimerManager().SetTimer(
				//	DotManagerHandle,
				//	FTimerDelegate::CreateLambda([&]()  // [=] 값 캡처
				//		{
				//			//ServerRPCApplyTargetDamage(DotHitTarget, Potency, DamageEvent, GetController(), this);
				//			RepeatingDamage();
				//		}
				//	), 1.0f, true, 1.0f
				//);
				

				FTimerHandle DummyHandle;
				DotHandle.Add(DummyHandle);

				uint32 Num = TimerNum;
				FTimerDelegate TimerDel;
				TimerDel.BindUFunction(this, FName("RepeatingDamage"), Num);

				GetWorld()->GetTimerManager().SetTimer(DotManagerHandle, TimerDel, 1.0f, true, 1.0f);
				++TimerNum;

				GetWorld()->GetTimerManager().SetTimer(
					DurationHandle,
					FTimerDelegate::CreateLambda([&]()
						{
							GetWorld()->GetTimerManager().ClearTimer(DotManagerHandle);
							bIsThunder = false;
						}
					), 5.0f, false
				);

				//GetWorld()->GetTimerManager().SetTimer(
				//	DurationHandle,
				//	FTimerDelegate::CreateLambda([&]()  // [=] 값 캡처
				//		{
				//			GetWorld()->GetTimerManager().ClearTimer(DotHandle[TimerNum - 1]);
				//		}
				//	), 5.0f, false
				//);

				//GetWorld()->GetTimerManager().SetTimer(
				//	DoTHandle,
				//	FTimerDelegate::CreateLambda([&]()
				//		{
				//			GetWorldTimerManager().ClearTimer(DoTHandle);
				//		}
				//	), 3.0f, false
				//);
			}

			if (HitDetected)
			{
				float CurrentAttackDamage;

				if (bIsUsingSkill)
				{
					CurrentAttackDamage = CurrentDamage;
				}
				else
				{
					CurrentAttackDamage = Stat->GetTotalStat().Attack;
				}

				FDamageEvent DamageEvent;

				ServerRPCApplyDamage(OutHitResult, CurrentAttackDamage, DamageEvent, GetController(), this);

				UE_LOG(LogTemp, Log, TEXT("AttackDamage: %f"), CurrentAttackDamage);
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
				CurrentAttackDamage = CurrentDamage;
				HitTarget->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);

				uint32 Duration = DotTime;

				// 타겟 해제 방지용
				DotHitTarget = HitTarget;

				FTimerHandle DummyHandle;
				DotHandle.Add(DummyHandle);

				uint32 Num = TimerNum;
				FTimerDelegate TimerDel;
				TimerDel.BindUFunction(this, FName("RepeatingDamage"), Num);

				GetWorld()->GetTimerManager().SetTimer(DotManagerHandle, TimerDel, 1.0f, true, 1.0f);
				++TimerNum;

				GetWorld()->GetTimerManager().SetTimer(
					DurationHandle,
					FTimerDelegate::CreateLambda([&]()
						{
							GetWorld()->GetTimerManager().ClearTimer(DotManagerHandle);
						}
					), 5.5f, false
				);
			}

			FColor DebugColor = HitDetected ? FColor::Green : FColor::Red;
			if (HitDetected)
			{
				if (HasAuthority())
				{
					float CurrentAttackDamage;
					if (bIsUsingSkill)
					{
						CurrentAttackDamage = CurrentDamage;
					}
					else
					{
						CurrentAttackDamage = Stat->GetTotalStat().Attack;
					}
					FDamageEvent DamageEvent;
					OutHitResult.GetActor()->TakeDamage(CurrentAttackDamage, DamageEvent, GetController(), this);
				}
			}
		}
	}
}

void AP4WCharacterPlayer_BLM::RepeatingFunction()
{
	FTimerHandle Handle;
	uint32 RepeatingNum = 5;

	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda([&]()  // [=] 값 캡처
			{
				FDamageEvent DamageEvent;
				ServerRPCApplyTargetDamage(DotHitTarget, Potency, DamageEvent, GetController(), this);

				CountRepeatingCall(Handle, &RepeatingNum);
			}
		), 1.0f, true, 1.5f
	);
}

void AP4WCharacterPlayer_BLM::CountRepeatingCall(FTimerHandle Handle, uint32* Num)
{
	if (--(*Num) <= 0)
	{
		GetWorldTimerManager().ClearTimer(Handle);
	}

	//if (--RepeatingCallsRemaining <= 0)
	
}

void AP4WCharacterPlayer_BLM::RepeatingDamage(uint32 Num)
{
	FDamageEvent DamageEvent;
	ServerRPCApplyTargetDamage(DotHitTarget, Potency, DamageEvent, GetController(), this);
}

void AP4WCharacterPlayer_BLM::SpawnBlizzardTrailEffect(AActor* TargetActor)
{
	if (BlizzardTrailEffect && TargetActor)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			BlizzardTrailEffect,
			TargetActor->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}
}

void AP4WCharacterPlayer_BLM::ServerRPCBlizzardVFX_Implementation(UNiagaraSystem* NS, AActor* DamagedActor)
{
	ACharacter* Char = Cast<ACharacter>(DamagedActor);

	BlizzardImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NS,
		Char->GetMesh(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);
}

void AP4WCharacterPlayer_BLM::MulticastRPCBlizzardVFX_Implementation(UNiagaraSystem* NS, AActor* DamagedActor)
{
	ACharacter* Char = Cast<ACharacter>(DamagedActor);

	BlizzardImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NS,
		Char->GetMesh(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);
	ServerRPCFireVFX(NS, DamagedActor);
}

void AP4WCharacterPlayer_BLM::ServerRPCFireVFX_Implementation(UNiagaraSystem* NS, AActor* DamagedActor)
{
	if (MagicProjectileClass && DamagedActor)
	{
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f;
		FRotator SpawnRotation = (DamagedActor->GetActorLocation() - SpawnLocation).Rotation();

		AMagicProjectile* Projectile = GetWorld()->SpawnActor<AMagicProjectile>(MagicProjectileClass, SpawnLocation, SpawnRotation);
		if (Projectile)
		{
			Projectile->LaunchToTarget(DamagedActor);
		}
	}
}

void AP4WCharacterPlayer_BLM::MulticastRPCFireVFX_Implementation(UNiagaraSystem* NS, AActor* DamagedActor)
{
	// Projectile 발사
	if (MagicProjectileClass && DamagedActor)
	{
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.f;
		FRotator SpawnRotation = (DamagedActor->GetActorLocation() - SpawnLocation).Rotation();

		AMagicProjectile* Projectile = GetWorld()->SpawnActor<AMagicProjectile>(MagicProjectileClass, SpawnLocation, SpawnRotation);
		if (Projectile)
		{
			Projectile->LaunchToTarget(DamagedActor);
		}
	}
}

void AP4WCharacterPlayer_BLM::ServerRPCThunderVFX_Implementation(UNiagaraSystem* NS, AActor* DamagedActor)
{
	ACharacter* Char = Cast<ACharacter>(DamagedActor);

	ThunderImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NS,
		Char->GetMesh(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);
}

void AP4WCharacterPlayer_BLM::MulticastRPCThunderVFX_Implementation(UNiagaraSystem* NS, AActor* DamagedActor)
{
	ACharacter* Char = Cast<ACharacter>(DamagedActor);

	ThunderImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NS,
		Char->GetMesh(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);

	//ThunderImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS, DamagedActor->GetActorLocation());
	//ThunderImpactEffectComponent->Activate();
}

void AP4WCharacterPlayer_BLM::ServerRPCFireBallVFX_Implementation(UNiagaraSystem* NS, AActor* DamagedActor)
{
	ACharacter* Char = Cast<ACharacter>(DamagedActor);

	FireBallImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NS,
		Char->GetMesh(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);

	//MulticastRPCFireBallVFX(NS, DamagedActor);
}

void AP4WCharacterPlayer_BLM::MulticastRPCFireBallVFX_Implementation(UNiagaraSystem* NS, AActor* DamagedActor)
{
	ACharacter* Char = Cast<ACharacter>(DamagedActor);

	FireBallImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NS,
		Char->GetMesh(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTargetIncludingScale,
		true
	);

	//FireBallImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS, DamagedActor->GetActorLocation());
	//FireBallImpactEffectComponent->Activate();
}

void AP4WCharacterPlayer_BLM::PlayBlizzardAttackAnimation(int32 Time)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FireAttackMontage);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda([&]()
			{
				//SpawnBlizzardImpactEffect(HitTarget->GetActorLocation(), HitTarget->GetActorLocation());

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
				bIsUsingSkill = false;
				bCanAttack = true;
				NotifyComboActionEnd();
			}
		), Time + 0.5f, false
	);
	//AnimInstance->Montage_JumpToSection(ComboNumber, ComboAttackMontage);
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
				bIsUsingSkill = false;
				bCanAttack = true;
				NotifyComboActionEnd();
			}
		), Time + 0.5f, false
	);
	//AnimInstance->Montage_JumpToSection(ComboNumber, ComboAttackMontage);
}

void AP4WCharacterPlayer_BLM::PlayThunderAttackAnimation(int32 Time)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ThunderAttackMontage);
	NotifyComboActionEnd();
}

void AP4WCharacterPlayer_BLM::PlayFireBallAttackAnimation(int32 Time)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FireBallAttackMontage);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda([&]()
			{
				bIsCasting = false;

				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				AnimInstance->Montage_JumpToSection(TEXT("CastingEnd"), FireBallAttackMontage);
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
				bIsUsingSkill = false;
				bCanAttack = true;
				NotifyComboActionEnd();
			}
		), Time + 0.5f, false
	);
	//AnimInstance->Montage_JumpToSection(ComboNumber, ComboAttackMontage);
}

void AP4WCharacterPlayer_BLM::PlayManafontAnimation(int32 Time)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ManafontMontage);
	NotifyComboActionEnd();
}

// RPC
void AP4WCharacterPlayer_BLM::ServerRPCBlizzardAttack_Implementation(int32 Time)
{
	MulticastRPCBlizzardAttack(Time);
}

void AP4WCharacterPlayer_BLM::MulticastRPCBlizzardAttack_Implementation(int32 Time)
{
	if (!IsLocallyControlled())		// 로컬에서 실행되지 않으면(= 서버가 아니면)
	{
		PlayBlizzardAttackAnimation(Time);
	}
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

void AP4WCharacterPlayer_BLM::ServerRPCThunderAttack_Implementation(int32 Time, float DoTDuration, float DoTPotency)
{
	MulticastRPCThunderAttack(Time, DoTDuration, DoTPotency);
}

void AP4WCharacterPlayer_BLM::MulticastRPCThunderAttack_Implementation(int32 Time, float DoTDuration, float DoTPotency)
{
	if (!IsLocallyControlled())		// 로컬에서 실행되지 않으면(= 서버가 아니면)
	{
		PlayThunderAttackAnimation(Time);
	}
}

void AP4WCharacterPlayer_BLM::ServerRPCFireBallAttack_Implementation(int32 Time)
{
	MulticastRPCFireBallAttack(Time);
}

void AP4WCharacterPlayer_BLM::MulticastRPCFireBallAttack_Implementation(int32 Time)
{
	if (!IsLocallyControlled())		// 로컬에서 실행되지 않으면(= 서버가 아니면)
	{
		PlayFireBallAttackAnimation(Time);
	}
}

void AP4WCharacterPlayer_BLM::ServerRPCManafont_Implementation(int32 Time)
{
	MulticastRPCManafont(Time);
}

void AP4WCharacterPlayer_BLM::MulticastRPCManafont_Implementation(int32 Time)
{
	if (!IsLocallyControlled())		// 로컬에서 실행되지 않으면(= 서버가 아니면)
	{
		PlayManafontAnimation(Time);
	}
}