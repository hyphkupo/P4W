// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/P4WUserWidget.h"
#include "P4WExpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UP4WExpBarWidget : public UP4WUserWidget
{
	GENERATED_BODY()
	
public:
	UP4WExpBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxExp(float NewMaxExp) { MaxExp = NewMaxExp; }
	void UpdateExpBar(float NewCurrentExp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> ExpProgressBar;

	UPROPERTY()
	float MaxExp;
};
