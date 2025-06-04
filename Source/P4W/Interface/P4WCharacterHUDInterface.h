// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P4WCharacterHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UP4WCharacterHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class P4W_API IP4WCharacterHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetupHUDWidget(class UP4WHUDWidget* InHUDWidget) = 0;

};
