// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_LandStart.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotify_LandStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	//// �ִ� �ν��Ͻ��� ������ ��(����)�� ĳ���ͷ� ��ȯ
	//Owner = Cast<ACharacter>(GetOwningActor());

	//if (Owner)
	//{
	//	// �����Ʈ ������Ʈ �˻�
	//	Movement = Owner->GetCharacterMovement();
	//}
}
