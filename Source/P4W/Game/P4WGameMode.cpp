// Copyright Epic Games, Inc. All Rights Reserved.

#include "P4WGameMode.h"
#include "UObject/ConstructorHelpers.h"

AP4WGameMode::AP4WGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
