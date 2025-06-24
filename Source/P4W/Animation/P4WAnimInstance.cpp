// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/P4WAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UP4WAnimInstance::UP4WAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

// 애니메이션이 초기화될 때 호출
void UP4WAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 애님 인스턴스를 소유한 폰(액터)을 캐릭터로 변환
	Owner = Cast<ACharacter>(GetOwningActor());

	if (Owner)
	{
		// 무브먼트 컴포넌트 검색
		Movement = Owner->GetCharacterMovement();
		//Owner->GetCharacterMovement()->GroundFriction = 20.0f;
		//Movement->BrakingDecelerationWalking = 2050.0f;
	}
}

// 애니메이션 업데이트 함수
void UP4WAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 필요한 값 업데이트
	if (Movement)
	{
		//UE_LOG(LogTemp, Log, TEXT("Velocity.X: %f, Y: %f, Z: %f"), Velocity.X, Velocity.Y, Velocity.Z);
		Velocity = Movement->Velocity;
		
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold) & (Velocity.X > KINDA_SMALL_NUMBER) & (Velocity.Y > KINDA_SMALL_NUMBER);
		bIsJumpingPlace = bIsFalling & (Velocity.Z > JumpingThreshold) & (Velocity.X == 0.0f) & (Velocity.Y == 0.0f);

		//UE_LOG(LogTemp, Log, TEXT("Speed: %f"), GroundSpeed);
	}
}
