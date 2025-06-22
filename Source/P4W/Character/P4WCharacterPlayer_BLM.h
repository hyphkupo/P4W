// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/P4WCharacterBase.h"
#include "Interface/P4WCharacterHUDInterface.h"
#include "P4WCharacterPlayer_BLM.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API AP4WCharacterPlayer_BLM : public AP4WCharacterBase, public IP4WCharacterHUDInterface
{
	GENERATED_BODY()
	
public:
	AP4WCharacterPlayer_BLM();

	virtual void BeginPlay() override;

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
	TObjectPtr<class UInputAction> AnyKeyPressedAction;

protected:
	// skill
	void AutoAttack(const FInputActionValue& Value);

	void BlizzardAttack(const FInputActionValue& Value);
	void FireAttack(const FInputActionValue& Value);
	void ThunderAttack(const FInputActionValue& Value);
	void FireBallAttack(const FInputActionValue& Value);
	void Manafont(const FInputActionValue& Value);

	void AnyKeyPressed(const FInputActionValue& Value);

protected:
// Animation Montage
	// Blizzard, Fire Attack 공유
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> FireAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ThunderAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> FireBallAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ManafontMontage;

protected:
// Play AnimMontage
	void PlayAutoAttackAnimation();
	void PlayBlizzardAttackAnimation(int32 Time);
	void PlayFireAttackAnimation(int32 Time);
	void PlayThunderAttackAnimation(int32 Time);
	void PlayFireBallAttackAnimation(int32 Time);
	void PlayManafontAnimation(int32 Time);

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
	virtual void AttackHitCheck() override;
	virtual void SpellHitCheck() override;
	virtual void SpellHitCheckDoT() override;

	void DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward);

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

protected:
	virtual void SetupHUDWidget(UP4WHUDWidget* InHUDWidget) override;

protected:
	// 프로퍼티 리플리케이션 등록을 위한 함수 오버라이딩
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// RPC
	// Blizzard Attack
	UFUNCTION(Server, Unreliable)
	void ServerRPCBlizzardAttack(int32 Time);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCBlizzardAttack(int32 Time);

	// Fire Attack
	UFUNCTION(Server, Unreliable)
	void ServerRPCFireAttack(int32 Time);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCFireAttack(int32 Time);

	// Thunder Attack
	UFUNCTION(Server, Unreliable)
	void ServerRPCThunderAttack(int32 Time, float DoTDuration, float DoTPotency);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCThunderAttack(int32 Time, float DoTDuration, float DoTPotency);

	// FireBall Attack
	UFUNCTION(Server, Unreliable)
	void ServerRPCFireBallAttack(int32 Time);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCFireBallAttack(int32 Time);

	// Manafont
	UFUNCTION(Server, Unreliable)
	void ServerRPCManafont(int32 Time);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCManafont(int32 Time);

	UPROPERTY(Replicated)
	uint8 bIsInCombo : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UP4WPLDComboActionData> ComboActionData;

	float AcceptCheckDistance = 300.0f;
	float PrevDamage;

public:
	// 쿨타임
	float CooldownTime;

	// 캐스팅 시간
	UPROPERTY(Replicated)
	float CastingTime;

	// DoT
	// 도트 지속 시간
	float DotTime;

	// 도트 딜
	float Potency;

protected:
	bool CheckWasMappingKeyPressed();
	uint8 bIsAnyKeyPressed : 1;
	uint8 bIsCasting : 1;
	uint8 bIsUsingSkill : 1;
	uint8 bIsUsingAutoAttack : 1;

public:
	FTimerHandle CooldownHandle_BlizzardAttack;
	FTimerHandle CooldownHandle_FireAttack;
	FTimerHandle CooldownHandle_ThunderAttack;
	FTimerHandle CooldownHandle_FireBallAttack;
	FTimerHandle CooldownHandle_Manafont;

	uint8 bCanPlayBlizzardAttack : 1;
	uint8 bCanPlayFireAttack : 1;
	uint8 bCanPlayThunderAttack : 1;
	uint8 bCanPlayFireBallAttack : 1;
	uint8 bCanPlayManafont : 1;

protected:
	//// 타겟 해제 방지용
	//UPROPERTY()
	//TObjectPtr<class AActor> DotHitTarget;
	
	TArray<FTimerHandle> DotHandle;
	uint32 TimerNum = 0;

	void RepeatingFunction();

	void CountRepeatingCall(FTimerHandle Handle, uint32* Num);
	//uint32 RepeatingNum;

	FTimerHandle DotManagerHandle;
	FTimerHandle DurationHandle;


	UFUNCTION()
	void RepeatingDamage(uint32 Num);

	uint8 bIsThunder : 1;

	// VFX
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* ManafontVFXSystem;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "VFX")
	UNiagaraComponent* ManafontVFXComponent;

protected:
	void SpawnBlizzardTrailEffect(AActor* TargetActor);
	//void SpawnBlizzardImpactEffect(FVector Location, FVector Normal);

protected:
	// Blizzard
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* BlizzardTrailEffect;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* BlizzardImpactEffect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "VFX")
	UNiagaraComponent* BlizzardImpactEffectComponent;

	// Fire
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* FireImpactEffect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "VFX")
	UNiagaraComponent* FireImpactEffectComponent;

	// Thunder
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* ThunderImpactEffect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "VFX")
	UNiagaraComponent* ThunderImpactEffectComponent;

	// FireBall
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* FireBallImpactEffect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "VFX")
	UNiagaraComponent* FireBallImpactEffectComponent;

	UPROPERTY()
	TObjectPtr<class AMagicProjectile> Spell;

protected:
	// Blizzard
	UFUNCTION(Server, Unreliable)
	void ServerRPCBlizzardVFX(UNiagaraSystem* NS, AActor* DamagedActor);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCBlizzardVFX(UNiagaraSystem* NS, AActor* DamagedActor);
	
	// Fire
	UFUNCTION(Server, Unreliable)
	void ServerRPCFireVFX(UNiagaraSystem* NS, AActor* DamagedActor);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCFireVFX(UNiagaraSystem* NS, AActor* DamagedActor);

	// Thunder
	UFUNCTION(Server, Unreliable)
	void ServerRPCThunderVFX(UNiagaraSystem* NS, AActor* DamagedActor);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCThunderVFX(UNiagaraSystem* NS, AActor* DamagedActor);

	// FireBall
	UFUNCTION(Server, Unreliable)
	void ServerRPCFireBallVFX(UNiagaraSystem* NS, AActor* DamagedActor);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCFireBallVFX(UNiagaraSystem* NS, AActor* DamagedActor);

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class AMagicProjectile> MagicProjectileClass;

	

};
