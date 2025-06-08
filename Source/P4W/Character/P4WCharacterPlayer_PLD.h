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

// Animation Montage
protected:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UAnimMontage> ComboAttackMontage;

protected:
	//UPROPERTY()
	//int32 ComboNum;

	virtual void SetupHUDWidget(UP4WHUDWidget* InHUDWidget) override;
};
