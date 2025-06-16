// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/P4WGameSingleton.h"

DEFINE_LOG_CATEGORY(LogP4WGameSingleton);

UP4WGameSingleton::UP4WGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/GameData/P4WCharacterStatTable.P4WCharacterStatTable"));

	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FP4WCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
	//MaxEnmity = 0.0f;
}

UP4WGameSingleton& UP4WGameSingleton::Get()
{
	UP4WGameSingleton* Singleton = CastChecked<UP4WGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogP4WGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UP4WGameSingleton>();
}
