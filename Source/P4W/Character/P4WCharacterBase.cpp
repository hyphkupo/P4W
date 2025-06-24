// Copyright Epic Games, Inc. All Rights Reserved.

#include "P4WCharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStat/P4WCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"

#include "Animation/AnimMontage.h"

#include "Net/UnrealNetwork.h"

#include "EngineUtils.h"
#include "P4W.h"

#include "UI/P4WUserWidget.h"
#include "UI/P4WHpBarWidget.h"
#include "UI/P4WMpBarWidget.h"
#include "UI/P4WCooltimeWidget.h"
#include "UI/P4WWidgetComponent.h"

#include "Physics/P4WCollision.h"
#include "Engine/DamageEvents.h"

#include "Skill/SkillSystemComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Monster/P4WBoss.h"
#include "Game/P4WGameInstance.h"

#include "NiagaraFunctionLibrary.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"

//#include "GameFramework/PlayerStart.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AP4WCharacterBase::AP4WCharacterBase()
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

	//static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContext2Ref(TEXT("/Game/Input/IMC_2.IMC_2"));
	//if (DefaultMappingContext2Ref.Object)
	//{
	//	DefaultMappingContext2 = DefaultMappingContext2Ref.Object;
	//}

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

	static ConstructorHelpers::FObjectFinder<UInputAction> TargetingActionRef(TEXT("/Game/Input/IA_Targeting.IA_Targeting"));
	if (TargetingActionRef.Object)
	{
		TargetingAction = TargetingActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TargetingSelfActionRef(TEXT("/Game/Input/IA_TargetingSelf.IA_TargetingSelf"));
	if (TargetingSelfActionRef.Object)
	{
		TargetingSelfAction = TargetingSelfActionRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> CancelTargetingActionRef(TEXT("/Game/Input/IA_CancelTargeting.IA_CancelTargeting"));
	if (CancelTargetingActionRef.Object)
	{
		CancelTargetingAction = CancelTargetingActionRef.Object;
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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/Animation/AM_Dead.AM_Dead"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadBossMontageRef(TEXT("/Game/Animation/AM_DeadBoss.AM_DeadBoss"));
	if (DeadBossMontageRef.Object)
	{
		DeadBossMontage = DeadBossMontageRef.Object;
	}

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -27.5f));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(25.0f, 25.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_P4WCAPSULE);
		
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
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 90.0f);
	//SpringArm->TargetOffset = FVector(0.0f, 0.0f, 90.0f);
	//SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	
	// 벽에 부딪히면 자동으로 카메라를 앞으로 이동하도록 하는 변수
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	// Stat Component
	Stat = CreateDefaultSubobject<UP4WCharacterStatComponent>(TEXT("Stat"));

	// Widget Component
	HpBar = CreateDefaultSubobject<UP4WWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 65.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	HpBar->bHiddenInGame = true;

	//CooltimeBar = CreateDefaultSubobject<UP4WWidgetComponent>(TEXT("Widget"));


	// Skill Component
	Skill = CreateDefaultSubobject<USkillSystemComponent>(TEXT("Skill"));

	bReplicates = true;
	bCanAttack = true;

	// VFX
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> TargetingVFXRef(TEXT("/Game/TrackMarker/Niagara/NotLooping/Basic/NS_Icon2D_Arrow_12.NS_Icon2D_Arrow_12"));
	if (TargetingVFXRef.Object)
	{
		TargetingVFXSystem = TargetingVFXRef.Object;
	}
}

void AP4WCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AP4WCharacterBase::SetDead);
}

void AP4WCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		//PlayerController->GetPawn()->SetActorLocation(FVector(FMath::RandRange(10.0f, 30.0f), 30.0f, 230.0f));

		EnableInput(PlayerController);

		// IMC 선택
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	/*
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
	*/

}

void AP4WCharacterBase::PostNetInit()
{
	Super::PostNetInit();
}

int32 AP4WCharacterBase::GetLevel()
{
	return Stat->GetCurrentLevel();
}

void AP4WCharacterBase::SetLevel(int32 InNewLevel)
{
	Stat->SetLevelStat(InNewLevel);
}

void AP4WCharacterBase::SetupCharacterWidget(UP4WUserWidget* InUserWidget)
{
	UP4WHpBarWidget* HpBarWidget = Cast<UP4WHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp(), Stat->GetMaxHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UP4WHpBarWidget::UpdateHpBar);
	}

	UP4WMpBarWidget* MpBarWidget = Cast<UP4WMpBarWidget>(InUserWidget);
	if (MpBarWidget)
	{
		MpBarWidget->UpdateMpBar(Stat->GetCurrentMp(), Stat->GetMaxMp());
		Stat->OnMpChanged.AddUObject(MpBarWidget, &UP4WMpBarWidget::UpdateMpBar);
	}

	UP4WCooltimeWidget* CooltimeBarWidget = Cast<UP4WCooltimeWidget>(InUserWidget);
	if (CooltimeBarWidget)
	{
		//CooltimeBarWidget->UpdateCooltimeBar();
		//Stat->OnMpChanged.AddUObject(MpBarWidget, &UP4WMpBarWidget::UpdateMpBar);
	}
}

void AP4WCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AP4WCharacterBase, bCanAttack);
	DOREPLIFETIME(AP4WCharacterBase, CurrentDamage);
	//DOREPLIFETIME(AP4WCharacterBase, ComboNum);
	//DOREPLIFETIME(AP4WCharacterBase, HitTarget);
	//DOREPLIFETIME(AP4WCharacterBase, MaxEnmity);
}

void AP4WCharacterBase::ClientRPCAutoAttack_Implementation()
{
}

void AP4WCharacterBase::ServerRPCAutoAttack_Implementation()
{
	PlayAutoAttackAnimation();
	MulticastRPCAutoAttack();
}

void AP4WCharacterBase::MulticastRPCAutoAttack_Implementation()
{
	if (!IsLocallyControlled())		// 로컬에서 실행되지 않으면(= 서버가 아니면)
	{
		PlayAutoAttackAnimation();
	}
}

void AP4WCharacterBase::ClientRPCComboAttack_Implementation(AP4WCharacterBase* CharacterToPlay)
{
	if (CharacterToPlay)
	{
		CharacterToPlay->PlayComboAttackAnimation(ComboNum);
	}
	//MulticastRPCAutoAttack();
}

void AP4WCharacterBase::ServerRPCComboAttack_Implementation(int32 Num)
{
	//UE_LOG(LogTemp, Log, TEXT("ServerRPCAutoAttack Begin"));
	PlayComboAttackAnimation(Num);
	MulticastRPCComboAttack(Num);

	//for (auto* PlayerController : TActorRange<APlayerController>(GetWorld()))
	//{
	//	// 서버에 있는 클라이언트에 해당하는 플레이어 컨트롤러 거르기.
	//	if (PlayerController && GetController() != PlayerController)
	//	{
	//		// 클라이언트 중에서 본인이 아닌지 확인.
	//		// 리슨서버의 컨트롤러 거르기.
	//		if (!PlayerController->IsLocalController())
	//		{
	//			// 여기로 넘어온 플레이어 컨트롤러는
	//			// 서버도 아니고, 본인 클라이언트도 아님.
	//			AP4WCharacterBase* OtherPlayer = Cast<AP4WCharacterBase>(PlayerController);
	//			if (OtherPlayer)
	//			{
	//				// Client RPC 전송.
	//				//ClientRPCPlayAnimation(OtherPlayer);
	//				OtherPlayer->ClientRPCAutoAttack(this);
	//			}
	//		}
	//	}
	//}
	//UE_LOG(LogTemp, Log, TEXT("ServerRPCAutoAttack End"));
}

void AP4WCharacterBase::MulticastRPCComboAttack_Implementation(int32 Num)
{
	if (!IsLocallyControlled())		// 로컬에서 실행되지 않으면(= 서버가 아니면)
	{
		PlayComboAttackAnimation(Num);
	}
	//UE_LOG(LogTemp, Log, TEXT("[%s] Multicast ComboNum: %d"), LOG_NETMODEINFO, ComboNum);
}

void AP4WCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
	// Character Section
		// Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AP4WCharacterBase::Move);

		// Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		
	// Camera Section
		// Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AP4WCharacterBase::Look);
		
		// Zoom
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AP4WCharacterBase::Zoom);

		// Targeting
		EnhancedInputComponent->BindAction(TargetingAction, ETriggerEvent::Triggered, this, &AP4WCharacterBase::Targeting);
		EnhancedInputComponent->BindAction(TargetingSelfAction, ETriggerEvent::Triggered, this, &AP4WCharacterBase::TargetingSelf);
		EnhancedInputComponent->BindAction(CancelTargetingAction, ETriggerEvent::Triggered, this, &AP4WCharacterBase::CancelTargeting);
	}
}

void AP4WCharacterBase::Move(const FInputActionValue& Value)
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

void AP4WCharacterBase::Jump(const FInputActionValue& Value)
{
}

void AP4WCharacterBase::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y * (-1.0f));
	}
}

void AP4WCharacterBase::Zoom(const FInputActionValue& Value)
{
	FVector ZoomVector = Value.Get<FVector>();
	float ChangeLength;
	ChangeLength = SpringArm->TargetArmLength - ZoomVector.X * 10.0f;
	SpringArm->TargetArmLength = FMath::Clamp(ChangeLength, 50.0f, 800.0f);
}

void AP4WCharacterBase::Targeting(const FInputActionValue& Value)
{
	FindTarget();
	if (HitTarget)
	{
		AP4WCharacterBase* Char = Cast<AP4WCharacterBase>(HitTarget);
		if (Char)
		{
			TargetingVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				TargetingVFXSystem,
				Char->GetMesh(),
				NAME_None,
				FVector(0.0f, 0.0f, 50.0f),
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset,
				true
			);
		}

		//TargetingVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TargetingVFXSystem, HitTarget->GetActorLocation());
		if (TargetingVFXComponent)
		{
			TargetingVFXComponent->Activate();
		}
		AP4WCharacterBase* HitTargetChar = Cast<AP4WCharacterBase>(HitTarget);
		if (HitTargetChar)
		{
			HitTargetChar->HpBar->bHiddenInGame = false;
		}
	}
	//ConeDetectWithDotProduct();
}

void AP4WCharacterBase::TargetingSelf(const FInputActionValue& Value)
{
	HitTarget = this;
	if (HitTarget)
	{
		AP4WCharacterBase* Char = Cast<AP4WCharacterBase>(HitTarget);
		if (Char)
		{
			TargetingVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				TargetingVFXSystem,
				Char->GetMesh(),
				NAME_None,
				FVector(0.0f, 0.0f, 50.0f),
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset,
				true
			);
		}

		//TargetingVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TargetingVFXSystem, HitTarget->GetActorLocation());
		if (TargetingVFXComponent)
		{
			TargetingVFXComponent->Activate();
		}
		UE_LOG(LogTemp, Log, TEXT("최종 타겟 액터: %s"), *HitTarget->GetName());
	}
}

void AP4WCharacterBase::CancelTargeting(const FInputActionValue& Value)
{
	AP4WCharacterBase* HitTargetChar = Cast<AP4WCharacterBase>(HitTarget);
	if (HitTargetChar)
	{
		TargetingVFXComponent->Deactivate();
		HitTargetChar->HpBar->bHiddenInGame = true;
	}
	HitTarget = nullptr;
}

void AP4WCharacterBase::PlayAutoAttackAnimation()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AutoAttackMontage);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateLambda([&]()
			{
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			}
		), 1.0f, false
	);
}

void AP4WCharacterBase::PlayComboAttackAnimation(int32 Num)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboAttackMontage);
	
	FTimerHandle Handle;
	if (Num == 3)
	{
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateLambda([&]()
				{
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
				}
			), 1.7f, false
		);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			FTimerDelegate::CreateLambda([&]()
				{
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
				}
			), 1.0f, false
		);
	}
	
	UE_LOG(LogTemp, Log, TEXT("[%s] AttackAnimation ComboNum: %d"), LOG_NETMODEINFO, Num);
	FName ComboNumber = *FString::Printf(TEXT("ComboAttack%d"), Num);
	AnimInstance->Montage_JumpToSection(ComboNumber, ComboAttackMontage);
}

void AP4WCharacterBase::AttackHitCheck()
{
	/*
//	FHitResult OutHitResult;
//	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
//	const float AttackRange = Stat->GetTotalStat().AttackRange;
//	const float AttackRadius = Stat->GetAttackRadius();
//	float AttackDamage;
//	if (bIsInCombo)
//	{
//		AttackDamage = CurrentDamage;
//		UE_LOG(LogTemp, Log, TEXT("[%s] AttackDamagege: %f"), LOG_NETMODEINFO, AttackDamage);
//
//	}
//	else
//	{
//		AttackDamage = Stat->GetTotalStat().Attack;
//	}
//
//	//UE_LOG(LogTemp, Log, TEXT("Damage:%f"), AttackDamage);
//
//	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
//	const FVector End = Start + GetActorForwardVector() * AttackRange;
//
//	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_P4WACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
//	UE_LOG(LogTemp, Log, TEXT("AttackHitCheck"));
//
//	if (HitDetected)
//	{
//		UE_LOG(LogTemp, Log, TEXT("HitDetected"));
//		FDamageEvent DamageEvent;
//		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
//		UE_LOG(LogTemp, Log, TEXT("AttackDamage: %f"), AttackDamage);
//	}
//
//#if ENABLE_DRAW_DEBUG
//
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	float CapsuleHalfHeight = AttackRange * 0.5f;
//	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
//
//#endif
*/
}

void AP4WCharacterBase::SpellHitCheck()
{
}

void AP4WCharacterBase::SpellHitCheckDoT()
{
}

float AP4WCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	Stat->ApplyDamage(DamageAmount);
	
	return DamageAmount;
}

void AP4WCharacterBase::FindTarget()
{
	FVector MyLocation = GetActorLocation();
	FVector MyForward = GetActorForwardVector();

	float ViewAngleDegrees = 120.0f; // 전체 시야각
	float ViewRadius = 2000.0f;

	// 1. 구체 범위 내 액터 찾기
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	TArray<AActor*> OutActors;

	bool bFound = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		MyLocation,
		ViewRadius,
		ObjectTypes,
		nullptr,
		IgnoredActors,
		OutActors
	);

	// 2. Dot Product로 시야각 필터링
	for (AActor* Actor : OutActors)
	{
		if (!Actor)
		{
			continue;
		}

		FVector ToTarget = Actor->GetActorLocation() - MyLocation;
		ToTarget.Normalize();

		float Dot = FVector::DotProduct(MyForward, ToTarget); // 코사인 값 (1=정면, 0=수직, -1=반대)

		float CosHalfFOV = FMath::Cos(FMath::DegreesToRadians(ViewAngleDegrees * 0.5f));

		if (Dot >= CosHalfFOV)
		{
			// 이 액터는 시야각 내에 있음
			UE_LOG(LogTemp, Warning, TEXT("시야 내 감지: %s"), *Actor->GetName());
			HitActors.Add(Actor);
		}
	}

	float BaseActorDist = 2000.0f;
	for (AActor* Actor : HitActors)
	{
		float ActorDist = FVector::Distance(MyLocation, Actor->GetActorLocation());
		if (ActorDist < BaseActorDist)
		{
			HitTarget = Actor;
		}
		BaseActorDist = ActorDist;
	}
	HitActors.SetNum(0);

	if (HitTarget)
	{
		UE_LOG(LogTemp, Log, TEXT("최종 타겟 액터: %s"), *HitTarget->GetName());
	}
}

void AP4WCharacterBase::ProcessComboCommand()
{
	//if (ComboNum == 0)
	//{
	//	ComboActionBegin();
	//	return;
	//}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void AP4WCharacterBase::NotifyComboActionEnd()
{
}

void AP4WCharacterBase::SetMaxEnmity(float Enmity)
{
	UP4WGameInstance* GameInstance = Cast<UP4WGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->MaxEnmity = Enmity;
	}

	//if (HasAuthority())
	//{
	//	BossPtr->MaxEnmity = Enmity;
	//}
	//else
	//{
	//	BossPtr->MaxEnmity = Enmity;
	//	ServerRPCSetMaxEnmity(Enmity);
	//}
	UE_LOG(LogTemp, Log, TEXT("[%s]Enmity: %f"), LOG_NETMODEINFO, GameInstance->MaxEnmity);
}

void AP4WCharacterBase::SetDead()
{
	bIsDead = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	//APlayerController* PlayerController = Cast<APlayerController>(GetController());
	//if (PlayerController)
	//{
	//	DisableInput(PlayerController);
	//}
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
	AP4WBoss* BossPawn = Cast<AP4WBoss>(this);
	if (BossPawn)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			UBrainComponent* Brain = AIController->GetBrainComponent();
			if (Brain)
			{
				Brain->StopLogic(TEXT("Stop BT from code"));
			}
		}
	}
}

void AP4WCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AP4WBoss* BossPawn = Cast<AP4WBoss>(this);
	if (BossPawn)
	{
		AnimInstance->Montage_Play(DeadBossMontage, 1.0f);
	}
	else
	{
		AnimInstance->Montage_Play(DeadMontage, 1.0f);
	}
}

void AP4WCharacterBase::ServerRPCDeadAnimation_Implementation()
{
}

void AP4WCharacterBase::MulticastRPCDeadAnimation_Implementation()
{
	
}

void AP4WCharacterBase::MulticastRPCHowlingAnimation_Implementation(UAnimMontage* Montage)
{
	GetMesh()->GetAnimInstance()->Montage_Play(Montage);
}

void AP4WCharacterBase::MulticastRPCPushAnimation_Implementation(UAnimMontage* Montage)
{
	GetMesh()->GetAnimInstance()->Montage_Play(Montage);
}

void AP4WCharacterBase::ServerRPCSetMaxEnmity_Implementation(float Enmity)
{
	//MaxEnmity = Enmity;
}


//DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All)
//{
//}
