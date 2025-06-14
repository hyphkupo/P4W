// Copyright Epic Games, Inc. All Rights Reserved.

#include "P4WGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Player/P4WPlayerController.h"

AP4WGameMode::AP4WGameMode()
{
	// set default pawn class to our Blueprinted character
	//rannum = FMath::RandRange(0, 2);

	//if (rannum == 0)
	//{
	//	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/BP_PLDPlayer.BP_PLDPlayer_C"));
	//	if (PlayerPawnBPClass.Class != NULL)
	//	{
	//		DefaultPawnClass = PlayerPawnBPClass.Class;
	//	}
	//}
	//else
	//{
	//	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/BP_BLMPlayer.BP_BLMPlayer_C"));
	//	if (PlayerPawnBPClass.Class != NULL)
	//	{
	//		DefaultPawnClass = PlayerPawnBPClass.Class;
	//	}
	//}

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/BP_PLDPlayer.BP_PLDPlayer_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

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
//	//// �������� �ε��� ����.
//	//int32 RandIndex = FMath::RandRange(0, PlayerStartArray.Num() - 1);
//
//	//// ������ �ε����� �ش��ϴ� �÷��̾� ��ŸƮ ������ Ʈ������ ��ȯ.
//	//return PlayerStartArray[RandIndex]->GetActorTransform();
//	return FTransform();
//}

//void AP4WGameMode::StartPlay()
//{
//	Super::StartPlay();
//
//	// ���Ϳ� �ִ� �÷��̾� ��ŸƮ ���� ��ȸ.
//	for (APlayerStart* PlayerStart : TActorRange<APlayerStart>(GetWorld()))
//	{
//		// �迭�� �߰�.
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
