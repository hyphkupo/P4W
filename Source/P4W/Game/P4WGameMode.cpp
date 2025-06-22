// Copyright Epic Games, Inc. All Rights Reserved.

#include "P4WGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Player/P4WPlayerController.h"
#include "Monster/P4WBoss.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Player/P4WPlayerStart.h"
#include "Character/P4WCharacterPlayer_PLD.h"
#include "Character/P4WCharacterPlayer_BLM.h"
#include "Game/P4WGameInstance.h"

AP4WGameMode::AP4WGameMode()
{
	// set default pawn class to our Blueprinted character
	uint32 randnum = FMath::RandRange(0, 2);

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprint/BP_P4WPlayerController.BP_P4WPlayerController_C"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void AP4WGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AP4WGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	rannum = FMath::RandRange(0, 2);
}

void AP4WGameMode::RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
{
	//FTransform(FVector(FMath::RandRange(10.0f, 30.0f), 30.0f, 230.0f));
	//NewPlayer->GetPawn()->SetActorLocation(FVector(FMath::RandRange(10.0f, 30.0f), 30.0f, 230.0f));
}

void AP4WGameMode::BeginPlay()
{
	// 온라인 서브시스템 연결 확인용
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		OnlineSubsystem->GetSessionInterface();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Found Subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString())
			);
		}
	}

	//const FTransform SpawnTransform(
	//	GetActorLocation() + FVector::UpVector * 88.0f
	//);

	//const FTransform SpawnTransfrom = FVector(0.0f, 0.0f, 0.0f);

	//GetWorld()->SpawnActor(AABCharacterNonPlayer::StaticClass());
	// NPC 생성.
	//AP4WBoss* P4WBoss = GetWorld()->SpawnActorDeferred<AP4WBoss>(AP4WBoss::StaticClass(), SpawnTransform);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AP4WBoss* P4WBoss = GetWorld()->SpawnActor<AP4WBoss>(AP4WBoss::StaticClass(), FVector(4890.000000, -1340.000000, 195.000000), FRotator::ZeroRotator, SpawnParams);
	// ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams
}

AActor* AP4WGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	for (TActorIterator<AP4WPlayerStart> It(GetWorld()); It; ++It)
	{
		UP4WGameInstance* GameInstance = Cast<UP4WGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			AP4WPlayerStart* CustomStart = *It;
			if (IsValid(CustomStart))
			{
				float RandNum = FMath::RandRange(0, 1);
				if (RandNum <= 0.5f)
				{
					LastChosenPawnClass = CustomStart->CustomPLDClass;
					++(GameInstance->StartNum);

					return CustomStart;
				}
				else
				{
					LastChosenPawnClass = CustomStart->CustomBLMClass;
					++(GameInstance->StartNum);

					return CustomStart;
				}
			}
		}
	}

	LastChosenPawnClass = nullptr;
	return Super::ChoosePlayerStart_Implementation(Player);
}

APawn* AP4WGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	if (LastChosenPawnClass)
	{
		return GetWorld()->SpawnActor<APawn>(
			LastChosenPawnClass,
			StartSpot->GetActorLocation(),
			StartSpot->GetActorRotation()
		);
	}

	TSubclassOf<APawn> DefaultClass = GetDefaultPawnClassForController(NewPlayer);
	return GetWorld()->SpawnActor<APawn>(
		DefaultClass,
		StartSpot->GetActorLocation(),
		StartSpot->GetActorRotation()
	);
}

//void AP4WGameMode::RestartPlayer(AController* NewPlayer)
//{
//}

//AActor* AP4WGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
//{
//	Super::FindPlayerStart_Implementation(Player, IncomingName);
//
//
//
//	return ;
//}

//FTransform AP4WGameMode::GetRandomStartTransform() const
//{
//	//if (PlayerStartArray.Num() == 0)
//	//{
//	//	return FTransform(FVector(0.0f, 0.0f, 230.0f));
//	//}
//
//	//// 랜덤으로 인덱스 선택.
//	//int32 RandIndex = FMath::RandRange(0, PlayerStartArray.Num() - 1);
//
//	//// 선택한 인덱스에 해당하는 플레이어 스타트 액터의 트랜스폼 반환.
//	//return PlayerStartArray[RandIndex]->GetActorTransform();
//	return FTransform();
//}

//void AP4WGameMode::StartPlay()
//{
//	Super::StartPlay();
//
//	// 액터에 있는 플레이어 스타트 액터 순회.
//	for (APlayerStart* PlayerStart : TActorRange<APlayerStart>(GetWorld()))
//	{
//		// 배열에 추가.
//		PlayerStartArray.Add(PlayerStart);
//	}
//}

//void AP4WGameMode::RestartPlayer(AController* NewPlayer)
//{
//	if (HasAuthority())
//	{
//		for (auto* PlayerStart : GetWorld()->)
//	}
//}
//
////APawn* AP4WGameMode::SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot)
////{
////	Super::SpawnDefaultPawnFor(NewPlayer, StartSpot);
////	return nullptr;
////}
//
//void AP4WGameMode::InitStartSpot(AActor* StartSpot, AController* NewPlayer)
//{
//	Super::InitStartSpot(StartSpot, NewPlayer);
//
//	if (HasAuthority())
//	{
//		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
//		for (auto* PlayerStart : FoundActors)
//		{
//			APlayerStart* P = Cast<APlayerStart>(PlayerStart);
//			if (P)
//			{
//				//int32 RandLocation = RandRange(0, );
//				P->SetActorLocation(FVector(10.0f, 10.0f, 230.0f));
//			}
//		}
//	}
//}
