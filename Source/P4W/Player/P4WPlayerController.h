// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P4WPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API AP4WPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AP4WPlayerController();
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UP4WHUDWidget> P4WHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UP4WHUDWidget> P4WHUDWidget;

protected:
	//virtual void SetupHUDWidget(class UP4WHUDWidget* InHUDWidget) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UP4WCharacterStatComponent> Stat;
};
