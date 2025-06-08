// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/P4WUserWidget.h"
#include "P4WMpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UP4WMpBarWidget : public UP4WUserWidget
{
	GENERATED_BODY()
	
public:
	UP4WMpBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	void UpdateMpBar(float NewCurrentMp, float NewMaxMp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> MpProgressBar;

	UPROPERTY()
	float MaxMp;

	UPROPERTY()
	float CurrentMp;
};