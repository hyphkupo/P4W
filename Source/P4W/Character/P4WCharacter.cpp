// Copyright Epic Games, Inc. All Rights Reserved.

#include "P4WCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"

#include "EngineUtils.h"
//#include "GameFramework/PlayerStart.h"

#include "Animation/AnimMontage.h"

#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AP4WCharacter

AP4WCharacter::AP4WCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Game/Cat_Simple/Cat/Meshes/Mesh_Cat_Simple.Mesh_Cat_Simple"));
	if (CharacterMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextRef(TEXT("/Game/Input/IMC_Default.IMC_Default"));
	if (DefaultMappingContextRef.Object)
	{
		DefaultMappingContext = DefaultMappingContextRef.Object;
	}

// Moving Input
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Game/Input/IA_Move.IA_Move"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/Input/IA_Jump.IA_Jump"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}

// Looking Input
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Game/Input/IA_Look.IA_Look"));
	if (LookActionRef.Object)
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ZoomActionRef(TEXT("/Game/Input/IA_Zoom.IA_Zoom"));
	if (ZoomActionRef.Object)
	{
		ZoomAction = ZoomActionRef.Object;
	}

// Attack Input
	static ConstructorHelpers::FObjectFinder<UInputAction> AutoAttackActionRef(TEXT("/Game/Input/IA_AutoAttack.IA_AutoAttack"));
	if (AutoAttackActionRef.Object)
	{
		AutoAttackAction = AutoAttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Combo1AttackActionRef(TEXT("/Game/Input/IA_Combo1Attack.IA_Combo1Attack"));
	if (Combo1AttackActionRef.Object)
	{
		Combo1AttackAction = Combo1AttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Combo2AttackActionRef(TEXT("/Game/Input/IA_Combo2Attack.IA_Combo2Attack"));
	if (Combo2AttackActionRef.Object)
	{
		Combo2AttackAction = Combo2AttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Combo3AttackActionRef(TEXT("/Game/Input/IA_Combo3Attack.IA_Combo3Attack"));
	if (Combo3AttackActionRef.Object)
	{
		Combo3AttackAction = Combo3AttackActionRef.Object;
	}

	// Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/Animation/ABP_P4WCharacter1.ABP_P4WCharacter1_C"));
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AutoAttackMontageRef(TEXT("/Game/Animation/AM_AutoAttack.AM_AutoAttack"));
	if (AutoAttackMontageRef.Object)
	{
		AutoAttackMontage = AutoAttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboAttackMontageRef(TEXT("/Game/Animation/AM_PLDCombo.AM_PLDCombo"));
	if (ComboAttackMontageRef.Object)
	{
		ComboAttackMontage = ComboAttackMontageRef.Object;
	}

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -27.5f));

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(25.0f, 25.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	/*
	// Create a camera boom (pulls in towards the player if there is a collision)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	*/

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);
	//SpringArm->SocketOffset = FVector(0.0f, 40.0f, 0.0f);
	//SpringArm->SetRelativeLocation(FVector(0.0f, 70.0f, 90.0f));
	//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	
	// 벽에 부딪히면 자동으로 카메라를 앞으로 이동하도록 하는 변수
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	ComboNum = 0;
}

void AP4WCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AP4WCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		//PlayerController->GetPawn()->SetActorLocation(FVector(FMath::RandRange(10.0f, 30.0f), 30.0f, 230.0f));

		//PlayerController->PostSpawnInitialize
		EnableInput(PlayerController);
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//for (APlayerStart* PlayerStart : TActorRange<APlayerStart>(GetWorld()))
	//{
	//	PlayerStart->SetActorLocation(FVector(1000.0f, 30.0f, 230.0f));
	//}

	//FVector MyCharacterPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//for (APlayerController* PlayerController : TActorRange<APlayerStart>(GetWorld()))
	//{
	//	PlayerStart->SetActorLocation(FVector(1000.0f, 30.0f, 230.0f));
	//}

	//GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(FVector(30.0f, 30.0f, 100.0f));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Player Location: %s"), *MyCharacterPosition.ToString()));
}

void AP4WCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AP4WCharacter::ServerRPCAutoAttack_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("ServerRPCAutoAttack Begin"));
	//PlayAutoAttackAnimation();
	MulticastRPCAutoAttack();
	UE_LOG(LogTemp, Log, TEXT("ServerRPCAutoAttack End"));
}

void AP4WCharacter::MulticastRPCAutoAttack_Implementation()
{
	//if (!IsLocallyControlled())		// 로컬에서 실행되지 않으면(= 서버가 아니면)
	//{
	//	PlayAutoAttackAnimation();
	//}
	PlayAutoAttackAnimation();
}

void AP4WCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
	// Character Section
		// Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AP4WCharacter::Move);

		// Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		
	// Camera Section
		// Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AP4WCharacter::Look);
		
		// Zoom
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AP4WCharacter::Zoom);

	// Attack Section
		// Auto Attack
		EnhancedInputComponent->BindAction(AutoAttackAction, ETriggerEvent::Triggered, this, &AP4WCharacter::AutoAttack);

		EnhancedInputComponent->BindAction(Combo1AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacter::Combo1Attack);
		EnhancedInputComponent->BindAction(Combo2AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacter::Combo2Attack);
		EnhancedInputComponent->BindAction(Combo3AttackAction, ETriggerEvent::Triggered, this, &AP4WCharacter::Combo3Attack);
	}
}

void AP4WCharacter::Move(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Log, TEXT("Input"));
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AP4WCharacter::Jump(const FInputActionValue& Value)
{
}

void AP4WCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y * (-1.0f));
	}
}

void AP4WCharacter::Zoom(const FInputActionValue& Value)
{
	FVector ZoomVector = Value.Get<FVector>();
	float ChangeLength;
	ChangeLength = SpringArm->TargetArmLength - ZoomVector.X * 10.0f;
	SpringArm->TargetArmLength = FMath::Clamp(ChangeLength, 50.0f, 800.0f);
}

void AP4WCharacter::AutoAttack(const FInputActionValue& Value)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	DisableInput(PlayerController);

	PlayAutoAttackAnimation();

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda([&]()
			{
				EnableInput(Cast<APlayerController>(GetController()));
			}
		), 1.0f, false
	);

	ServerRPCAutoAttack();
}

void AP4WCharacter::Combo1Attack(const FInputActionValue& Value)
{
	ComboNum = 1;
	PlayComboAttackAnimation(ComboNum);
}

void AP4WCharacter::Combo2Attack(const FInputActionValue& Value)
{
	ComboNum = 2;
	PlayComboAttackAnimation(ComboNum);
}

void AP4WCharacter::Combo3Attack(const FInputActionValue& Value)
{
	ComboNum = 3;
	PlayComboAttackAnimation(ComboNum);
}

void AP4WCharacter::PlayAutoAttackAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AutoAttackMontage);
}

void AP4WCharacter::PlayComboAttackAnimation(int32 CurrentComboNum)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	GetMesh()->GetAnimInstance()->Montage_Play(ComboAttackMontage);

	FName ComboNumber = *FString::Printf(TEXT("ComboAttack%d"), CurrentComboNum);
	AnimInstance->Montage_JumpToSection(ComboNumber, ComboAttackMontage);
}

//void AP4WCharacter::ClientRPCPlayAnimation_Implementation(AP4WCharacter* CharacterToPlay)
//{
//	if (CharacterToPlay)
//	{
//		GetMesh()->GetAnimInstance()->Montage_Play(AutoAttackMontage);
//	}
//}

//DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All)
//{
//}
