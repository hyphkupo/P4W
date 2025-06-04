// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P4WHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UP4WHpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UP4WHpBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
};
