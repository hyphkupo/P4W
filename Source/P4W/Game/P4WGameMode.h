// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P4WGameMode.generated.h"

UCLASS()
class P4W_API AP4WGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP4WGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	/*
	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName = TEXT("")) override;

	// 게임 모드에서 리스폰 처리를 할 때 랜덤으로 스폰 위치를 가져올 때 사용할 함수.
	virtual FTransform GetRandomStartTransform() const;

	virtual void RestartPlayer(AController* NewPlayer) override;

	virtual APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot) override;

	ENGINE_API APawn* SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform);

	//Called from RestartPlayerAtPlayerStart, can be used to initialize the start spawn actor
	virtual void InitStartSpot(AActor* StartSpot, AController* NewPlayer);

	virtual void StartPlay() override;

	virtual void RestartPlayer(AController* NewPlayer) override;

	*/
	virtual void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) override;

	virtual void BeginPlay() override;
	
protected:
	// 플레이어 스타트 액터 배열.
	TArray<TObjectPtr<class APlayerStart>> PlayerStartArray;

	int32 rannum;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

private:
	UPROPERTY()
	TSubclassOf<APawn> LastChosenPawnClass;
	//uint32 StartNum = 0;

};
