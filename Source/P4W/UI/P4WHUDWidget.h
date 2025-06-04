// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UP4WHpBarWidget> HpBar;
};
