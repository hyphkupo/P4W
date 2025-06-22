// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/P4WUserWidget.h"
#include "P4WCooltimeWidget.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UP4WCooltimeWidget : public UP4WUserWidget
{
	GENERATED_BODY()
	
public:
	UP4WCooltimeWidget(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION()
	void StartCooltime(float InCooltime);

	void UpdateCooltimeBar();
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PbCooltimeBar;

public:
	void TickCooltime();

	FTimerHandle CooltimeTimerHandle;

	UPROPERTY()
	float MaxCooltime = 1.0f;
	
	UPROPERTY()
	float CurrentCooltime = 0.0f;

};
