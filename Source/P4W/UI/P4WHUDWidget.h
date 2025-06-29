// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStat/P4WCharacterStatComponent.h"
#include "P4WHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UP4WHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UP4WHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	//void FMarkComponentEndOfFrameUpdateState(const FP4WCharacterStat&)
	void UpdateHpBar(float NewCurrentHp, float NewMaxHp);
	void UpdateMpBar(float NewCurrentMp, float NewMaxMp);
	void UpdateExpBar(float NewCurrentExp, float NewMaxExp);
	void UpdateStat(const FP4WCharacterStat& BaseStat, const FP4WCharacterStat& ModifierStat);

	//UPROPERTY()
	//TObjectPtr<class UP4WCooltimeWidget> CooltimeBar;

	//UFUNCTION()
	//void StartCooltime(float Duration);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UP4WHpBarWidget> HpBar;
	
	UPROPERTY()
	TObjectPtr<class UP4WMpBarWidget> MpBar;
	
	UPROPERTY()
	TObjectPtr<class UP4WExpBarWidget> ExpBar;

	

	UPROPERTY()
	TObjectPtr<class UP4WCharacterStatWidget> CharacterStat;

};
