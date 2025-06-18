// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/P4WCharacterBase.h"
#include "Interface/P4WCharacterHUDInterface.h"
#include "Skill/SkillSystemComponent.h"
#include "P4WCharacterPlayer_PLD.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API AP4WCharacterPlayer_PLD : public AP4WCharacterBase, public IP4WCharacterHUDInterface
{
	GENERATED_BODY()
	
public:
	AP4WCharacterPlayer_PLD();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Attack Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AutoAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Combo1AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Combo2AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Combo3AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FAttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CAttackAction;

protected:
	void AutoAttack(const FInputActionValue& Value);
	void Combo1Attack(const FInputActionValue& Value);
	void Combo2Attack(const FInputActionValue& Value);
	void Combo3Attack(const FInputActionValue& Value);

	void HealUp(const FInputActionValue& Value);
	// Reduces damage taken
	void Sheltron(const FInputActionValue& Value);

	// placing yourself at the top of a target's enmity list while gaining additional enmity.
	void Provoke(const FInputActionValue& Value);

// Animation Montage
protected:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UAnimMontage> ComboAttackMontage;

	// 클라이언트에서 액터가 맞았을 때 서버로 판정을 보내는 함수.
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifyHit(const FHitResult& HitResult, float HitCheckTime);

	// 클라이언트에서 충돌 판정 후 미스가 발생했을 때 보내는 함수.
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCNotifyMiss(
		FVector_NetQuantize TraceStart,
		FVector_NetQuantize TraceEnd,
		FVector_NetQuantizeNormal TraceDir,
		float HitCheckTime
	);

	UFUNCTION(Server, Reliable)
	void ServerRPCApplyDamage(const FHitResult& HitResult, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCApplyTargetDamage(AActor* Actor, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

protected:
	//UPROPERTY()
	//int32 ComboNum;

	virtual void SetupHUDWidget(UP4WHUDWidget* InHUDWidget) override;

protected:
	virtual void AttackHitCheck() override;
	virtual void SpellHitCheck() override;
	virtual void SpellHitCheckDoT() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward);

	float AcceptCheckDistance = 300.0f;

public:
	// 쿨타임
	float CooldownTime;

public:
	UPROPERTY(VisibleAnywhere)
	USkillSystemComponent* SkillSystem;

protected:
	// 프로퍼티 리플리케이션 등록을 위한 함수 오버라이딩
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated)
	uint8 bIsInCombo : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UP4WPLDComboActionData> ComboActionData;

	void ComboActionBegin();
	void ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	//void SetComboCheckTimer();
	//void ComboCheck();

	//bool ComboCheck();

	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;
	uint32 PrevComboNum;
	float TimerElapsed;
	float PrevTimerElapsed;
	uint8 bCanNextCombo1 : 1;
	uint8 bCanNextCombo2 : 1;

	float PrevDamage;

public:
	// Sheltron
	UPROPERTY(Replicated)
	uint8 bIsSheltron : 1;
	uint8 bCanPlayProvoke : 1;

// Cooldown Section
protected:
	FTimerHandle CooldownHandle_Combo1;
	FTimerHandle CooldownHandle_Combo2;
	FTimerHandle CooldownHandle_Combo3;

	uint8 bCanPlayCombo1 : 1;
	uint8 bCanPlayCombo2 : 1;
	uint8 bCanPlayCombo3 : 1;

protected:
	float CastingTime;
	uint8 bIsUsingSkill : 1;
	uint8 bIsCasting : 1;
	uint8 bCanPlayHealUp : 1;

	FTimerHandle CooldownHandle_HealUp;

	void PlayHealUpAnimation(int32 Time);

	// HealUp Animation RPC
	UFUNCTION(Server, Unreliable)
	void ServerRPCHealUpAnimation(int32 Time);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCHealUpAnimation(int32 Time);

	// HealUp RPC
	UFUNCTION(Server, Unreliable)
	void ServerRPCHealUp(AP4WCharacterBase* Target);

	//UFUNCTION(NetMulticast, Unreliable)
	//void MulticastRPCHealUp();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> HealUpMontage;
	
};
