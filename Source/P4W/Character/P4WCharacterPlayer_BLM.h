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
	// skill
	void BlizzardAttack(const FInputActionValue& Value);
	void FireAttack(const FInputActionValue& Value);
	void ThunderAttack(const FInputActionValue& Value);
	void FireBallAttack(const FInputActionValue& Value);
	void LeyLines(const FInputActionValue& Value);

// Animation Montage
protected:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UAnimMontage> ComboAttackMontage;

protected:
	virtual void SetupHUDWidget(UP4WHUDWidget* InHUDWidget) override;
};
