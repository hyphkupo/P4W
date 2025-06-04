// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P4WMpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UP4WMpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UP4WMpBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxMp(float NewMaxMp) { MaxMp = NewMaxMp; }
	void UpdateMpBar(float NewCurrentMp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> MpProgressBar;

	UPROPERTY()
	float MaxMp;
};
