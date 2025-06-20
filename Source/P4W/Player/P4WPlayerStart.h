// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "P4WPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API AP4WPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Start")
	TSubclassOf<APawn> CustomPawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Start")
	TSubclassOf<class AP4WCharacterPlayer_PLD> CustomPLDClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Start")
	TSubclassOf<class AP4WCharacterPlayer_BLM> CustomBLMClass;
};
