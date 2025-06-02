// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/P4WGameSingleton.h"

DEFINE_LOG_CATEGORY(LogP4WGameSingleton);

UP4WGameSingleton::UP4WGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/GameData/P4WCharacterStatTable.P4WCharacterStatTable"));

	if (DataTableRef.Object)
	{
		// UDataTable�� Key/Value�� ������.
		// Map���� ������� �ʰ�, �迭�� ���� ����.
		const UDataTable* DataTable = DataTableRef.Object;

		// ������ ���̺��� ����� �ʱ�ȭ�ƴ��� Ȯ��(����).
		check(DataTable->GetRowMap().Num() > 0);

		// Value ���� �����ϱ� ���� �迭.
		TArray<uint8*> ValueArray;

		// ������ ���̺��� GenerateValueArray �Լ��� ����� Value ���� �迭�� ����.
		DataTable->GetRowMap().GenerateValueArray(ValueArray);

		// �˰����� Ȱ���� Value ���� ��ȯ�� ����.
		//for (uint8* Value : ValueArray)
		//{
		//	CharacterStatTable.Add(*reinterpret_cast<FABCharacterStat*>(Value));
		//}
		// �� for���� �Ʒ� ���پ��� ��� ����
		// ���ٴ� ��ü => ���ο��� �޸� �Ҵ� �ʿ��ϹǷ� ���ٰ� �� �ɸ� ����

		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FP4WCharacterStat*>(Value);
			}
		);
	}

	// ���� ������ ������ Ȯ��.
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
