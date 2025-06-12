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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> FireAttackMontage;

protected:
// Play AnimMontage
	void PlayAutoAttackAnimation();
	void PlayFireAttackAnimation(int32 Time);

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

	void DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward);

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

protected:
	virtual void SetupHUDWidget(UP4WHUDWidget* InHUDWidget) override;

protected:
	// 프로퍼티 리플리케이션 등록을 위한 함수 오버라이딩
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Unreliable)
	void ServerRPCFireAttack(int32 Time);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCFireAttack(int32 Time);

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

protected:
	bool CheckWasMappingKeyPressed();
	uint8 bIsAnyKeyPressed : 1;
	uint8 bIsCasting : 1;
	uint8 bIsUsingSkill : 1;

public:
	FTimerHandle CooldownHandle_FireAttack;
	uint8 bCanPlayFireAttack : 1;
};
