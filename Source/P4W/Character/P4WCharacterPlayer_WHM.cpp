// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/P4WCharacterPlayer_WHM.h"
#include "EnhancedInputComponent.h"

AP4WCharacterPlayer_WHM::AP4WCharacterPlayer_WHM()
{
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> BasicAttackRef(TEXT("/Game/Cat_Simple/Cat/Animations/Anim_Cat_Simple_SharpenClaws_Horiz.Anim_Cat_Simple_SharpenClaws_Horiz"));
	if (BasicAttackRef.Object)
	{
		BasicAttackAnimation = BasicAttackRef.Object;
	}
}

void AP4WCharacterPlayer_WHM::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
	// Attack Section
		// Combo Attack
		EnhancedInputComponent->BindAction(Combo1AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_WHM::Combo1Attack);
		EnhancedInputComponent->BindAction(Combo2AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_WHM::Combo2Attack);
		EnhancedInputComponent->BindAction(Combo3AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacterPlayer_WHM::Combo3Attack);
	}
}

void AP4WCharacterPlayer_WHM::BasicAttack(const FInputActionValue& Value)
{
	GetMesh()->PlayAnimation(BasicAttackAnimation, true);
}

void AP4WCharacterPlayer_WHM::DoTAttack(const FInputActionValue& Value)
{
}

void AP4WCharacterPlayer_WHM::SingleHeal(const FInputActionValue& Value)
{
}
