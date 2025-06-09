// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/P4WCharacterBase.h"
#include "Interface/P4WCharacterHUDInterface.h"
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

protected:
	//UPROPERTY()
	//int32 ComboNum;

	virtual void SetupHUDWidget(UP4WHUDWidget* InHUDWidget) override;

protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	float CooldownTime;
};
