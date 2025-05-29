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

	//virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName = TEXT("")) override;

	// ���� ��忡�� ������ ó���� �� �� �������� ���� ��ġ�� ������ �� ����� �Լ�.
	//virtual FTransform GetRandomStartTransform() const;

	//virtual void RestartPlayer(AController* NewPlayer) override;

	//virtual APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot) override;

	//ENGINE_API APawn* SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform);

	/** Called from RestartPlayerAtPlayerStart, can be used to initialize the start spawn actor */
	//virtual void InitStartSpot(AActor* StartSpot, AController* NewPlayer);

	//virtual void StartPlay() override;

	//virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) override;
	
protected:
	// �÷��̾� ��ŸƮ ���� �迭.
	TArray<TObjectPtr<class APlayerStart>> PlayerStartArray;
	
};
