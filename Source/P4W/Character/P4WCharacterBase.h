// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "UI/P4WHUDWidget.h"
#include "P4WCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AP4WCharacterBase : public ACharacter
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

// Attack Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AutoAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Combo1AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Combo2AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Combo3AttackAction;

protected:
// Called for input
// Movement Input
	void Move(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);

// Looking Input
	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);

// Attack Input
	void AutoAttack(const FInputActionValue& Value);
	
	void Combo1Attack(const FInputActionValue& Value);
	void Combo2Attack(const FInputActionValue& Value);
	void Combo3Attack(const FInputActionValue& Value);

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
	UPROPERTY()
	uint8 bCanAttack : 1;

	//// 이동 가능 여부 판정
	//UPROPERTY()
	//uint8 bCanMove;

protected:
	UPROPERTY()
	int32 ComboNum;

// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UP4WCharacterStatComponent> Stat;

// UI Widget Section
protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UWidgetComponent> HpBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

	void SetupHUDWidget(UP4WHUDWidget* InHUDWidget);

};
