// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "P4WAI.h"
#include "AIController.h"
#include "Interface/P4WCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/P4WCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Character/P4WCharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Monster/P4WBoss.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IP4WCharacterAIInterface* AIPawn = Cast<IP4WCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_P4WACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	bool bIsTarget = false;
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			// 폰을 조종하고 있는 것이 플레이어 컨트롤러인 경우(플레이어 캐릭터)에만 감지했다고 가정
			
			AP4WCharacterBase* PawnCharacter = Cast<AP4WCharacterBase>(Pawn);
			AP4WBoss* BossPawn = Cast<AP4WBoss>(AIPawn);

			if (Pawn && Pawn->GetController()->IsPlayerController() && (PawnCharacter->Stat->GetCurrentEnmity() >= BossPawn->MaxEnmity))
			{
				bIsTarget = true;

				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);

				//BossPawn->SetMaxEnmity(PawnCharacter->Stat->GetCurrentEnmity());
				UE_LOG(LogTemp, Log, TEXT("MaxEnmity: %f"), BossPawn->MaxEnmity);
				//CompareEnmity = BossPawn->MaxEnmity;

				//if (Pawn->HasAuthority())
				//{
				//	//ServerRPCSetEnmity(PawnCharacter->Stat->GetCurrentEnmity());
				//	MaxEnmity = PawnCharacter->Stat->GetCurrentEnmity();
				//	UE_LOG(LogTemp, Log, TEXT("MaxEnmity: %f"), MaxEnmity);
				//}
				//else
				//{
				//	ServerRPCSetEnmity(PawnCharacter->Stat->GetCurrentEnmity());
				//	UE_LOG(LogTemp, Log, TEXT("Client MaxEnmity: %f"), MaxEnmity);
				//}
			}
		}
	}

	if (!bIsTarget)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}
}

//void UBTService_Detect::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(UBTService_Detect, MaxEnmity);
//}
//
//void UBTService_Detect::ServerRPCSetEnmity_Implementation(float Enmity)
//{
//	MaxEnmity = Enmity;
//	MulticastRPCSetEnmity(Enmity);
//}
//
//void UBTService_Detect::MulticastRPCSetEnmity_Implementation(float Enmity)
//{
//	MaxEnmity = Enmity;
//}
