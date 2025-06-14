// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "UI/P4WHUDWidget.h"
#include "UI/P4WUserWidget.h"
#include "Interface/P4WCharacterWidgetInterface.h"
#include "Interface/P4WAnimationAttackInterface.h"
#include "P4WCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AP4WCharacterBase : public ACharacter, public IP4WCharacterWidgetInterface, public IP4WAnimationAttackInterface
{
	GENERATED_BODY()

public:
	AP4WCharacterBase();

	virtual void PostInitializeComponents() override;

// Input
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext2;

// Movement Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

// Looking Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TargetingAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CancelTargetingAction;

protected:
// Called for input
// Movement Input
	void Move(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);

// Looking Input
	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);

	void Targeting(const FInputActionValue& Value);
	void CancelTargeting(const FInputActionValue& Value);

// Play AnimMontage
	void PlayAutoAttackAnimation();

	void PlayComboAttackAnimation(int32 Num);

protected:
	// 프로퍼티 리플리케이션 등록을 위한 함수 오버라이딩
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Auto Attack
	UFUNCTION(Client, Unreliable)
	void ClientRPCAutoAttack();

	UFUNCTION(Server, Unreliable)
	void ServerRPCAutoAttack();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCAutoAttack();
	
	// Combo Attack
	UFUNCTION(Client, Unreliable)
	void ClientRPCComboAttack(AP4WCharacterBase* CharacterToPlay);

	UFUNCTION(Server, Unreliable)
	void ServerRPCComboAttack(int32 Num);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCComboAttack(int32 Num);
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void PostNetInit() override;

public:
	///** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	///** Returns FollowCamera subobject **/
	//FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

// Animation Montage
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AutoAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ComboAttackMontage;

public:
	// 공격 가능 여부 판정
	UPROPERTY(Replicated)
	uint8 bCanAttack : 1;

	//// 이동 가능 여부 판정
	//UPROPERTY()
	//uint8 bCanMove;

protected:
	UPROPERTY()
	int32 ComboNum;

// Stat Section
protected:

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UP4WCharacterStatComponent> Stat;

public:
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);

// UI Widget Section
protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UWidgetComponent> HpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(UP4WUserWidget* InUserWidget) override;

// Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	virtual void SpellHitCheck() override;
	virtual void SpellHitCheckDoT() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkillSystemComponent> Skill;

public:
	UPROPERTY(Replicated)
	float CurrentDamage;

protected:
	void FindTarget();
	float TargetingRange = 30.0f;
	uint8 bHasLockTarget;

public:
	UPROPERTY()
	TObjectPtr<class AActor> HitTarget;
	TArray<class AActor*> HitActors;

	// 타겟 해제 방지용
	UPROPERTY()
	TObjectPtr<class AActor> DotHitTarget;

	//void ConeDetectWithDotProduct();

};
