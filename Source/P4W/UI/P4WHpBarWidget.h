// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/P4WUserWidget.h"
#include "P4WHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UP4WHpBarWidget : public UP4WUserWidget
{
	GENERATED_BODY()
	
public:
	UP4WHpBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	void UpdateHpBar(float NewCurrentHp, float NewMaxHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float CurrentHp;
};
