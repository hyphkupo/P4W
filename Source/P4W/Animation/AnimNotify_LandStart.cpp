// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_LandStart.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotify_LandStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	//// 애님 인스턴스를 소유한 폰(액터)을 캐릭터로 변환
	//Owner = Cast<ACharacter>(GetOwningActor());

	//if (Owner)
	//{
	//	// 무브먼트 컴포넌트 검색
	//	Movement = Owner->GetCharacterMovement();
	//}
}
