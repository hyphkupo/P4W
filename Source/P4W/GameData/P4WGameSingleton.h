// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "P4WCharacterStat.h"
#include "P4WGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogP4WGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class P4W_API UP4WGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UP4WGameSingleton();
	static UP4WGameSingleton& Get();

// Character Stat Data Section
public:
	FORCEINLINE FP4WCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FP4WCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

	UPROPERTY()
	float MaxEnmity = 0.0f;

private:
	TArray<FP4WCharacterStat> CharacterStatTable;

};
