// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/P4WGameSingleton.h"

DEFINE_LOG_CATEGORY(LogP4WGameSingleton);

UP4WGameSingleton::UP4WGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/GameData/P4WCharacterStatTable.P4WCharacterStatTable"));

	if (DataTableRef.Object)
	{
		// UDataTable는 Key/Value로 관리됨.
		// Map으로 사용하지 않고, 배열로 직접 관리.
		const UDataTable* DataTable = DataTableRef.Object;

		// 데이터 테이블이 제대로 초기화됐는지 확인(검증).
		check(DataTable->GetRowMap().Num() > 0);

		// Value 값만 저장하기 위한 배열.
		TArray<uint8*> ValueArray;

		// 데이터 테이블의 GenerateValueArray 함수를 사용해 Value 값만 배열에 저장.
		DataTable->GetRowMap().GenerateValueArray(ValueArray);

		// 알고리즘을 활용해 Value 값을 변환해 저장.
		//for (uint8* Value : ValueArray)
		//{
		//	CharacterStatTable.Add(*reinterpret_cast<FABCharacterStat*>(Value));
		//}
		// 위 for문은 아래 람다쓰는 방법 같음
		// 람다는 객체 => 내부에서 메모리 할당 필요하므로 람다가 더 걸릴 수도

		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FP4WCharacterStat*>(Value);
			}
		);
	}

	// 값에 문제가 없는지 확인.
	ensure(CharacterStatTable.Num() > 0);
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
