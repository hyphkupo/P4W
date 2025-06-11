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

#include "Physics/P4WCollision.h"
#include "Engine/DamageEvents.h"

#include "Skill/SkillSystemComponent.h"

#include "Kismet/KismetSystemLibrary.h"

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

	// Skill Component
	Skill = CreateDefaultSubobject<USkillSystemComponent>(TEXT("Skill"));

	// Widget Component
	//HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	//HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	
	//static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	//if (HpBarWidgetRef.Class)
	//{
	//	//HpBarWidget = CreateWidget<UUserWidget>(GetWorld(), HpBarWidgetRef.Class);
	//	//HpBarWidget->AddToViewport();

	//	HpBar->SetWidgetClass(HpBarWidgetRef.Class);
	//	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	//	HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
	//	HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}

	//static ConstructorHelpers::FClassFinder<UP4WHpBarWidget> HpBarRef(TEXT("/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	//if (HpBarRef.Class)
	//{
	//	HpBarWidget = CreateWidget<UUserWidget>(GetWorld(), HpBarRef.Class);
	//	//HpBar = HpBarRef.Object;
	//	HpBarWidget->AddToViewport();
	//}

	bReplicates = true;
	bCanAttack = true;
}

void AP4WCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
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
}

void AP4WCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AP4WCharacterBase, bCanAttack);
	DOREPLIFETIME(AP4WCharacterBase, CurrentDamage);
	//DOREPLIFETIME(AP4WCharacterBase, ComboNum);
	//DOREPLIFETIME(AP4WCharacterBase, HitTarget);
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
	//ConeDetectWithDotProduct();
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

float AP4WCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	Stat->ApplyDamage(DamageAmount);
	
	return DamageAmount;
}

//	/** Object Query functions **/
//	bool UKismetSystemLibrary::LineTraceSingleForObjects(const UObject * WorldContextObject, const FVector Start, const FVector End, const TArray<TEnumAsByte<EObjectTypeQuery> > &ObjectTypes, bool bTraceComplex, const TArray<AActor*>&ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult & OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
//	{
//		static const FName LineTraceSingleName(TEXT("LineTraceSingleForObjects"));
//		FCollisionQueryParams Params = ConfigureCollisionParams(LineTraceSingleName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);
//
//		FCollisionObjectQueryParams ObjectParams = ConfigureCollisionObjectParams(ObjectTypes);
//		if (ObjectParams.IsValid() == false)
//		{
//			UE_LOG(LogBlueprintUserMessages, Warning, TEXT("Invalid object types"));
//			return false;
//		}
//
//		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
//		bool const bHit = World ? World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParams, Params) : false;
//
//#if ENABLE_DRAW_DEBUG
//		DrawDebugLineTraceSingle(World, Start, End, DrawDebugType, bHit, OutHit, TraceColor, TraceHitColor, DrawTime);
//#endif
//
//		return bHit;
//	}

//void AP4WCharacterBase::FindTarget()
//{
//	FVector Start = GetActorLocation();
//	FVector End = Start + GetActorForwardVector() * 400.0f;
//
//	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
//	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); // 예: 캐릭터만 감지
//
//	TArray<AActor*> ActorsToIgnore;
//	ActorsToIgnore.Add(this);
//
//	FHitResult HitResult;
//
//	for (int i = 0; i < 120; ++i)
//	{
//		bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
//			GetWorld(),
//			Start,
//			End,
//			30.0f,
//			ObjectTypes,
//			false,                  // bTraceComplex
//			ActorsToIgnore,
//			EDrawDebugTrace::ForDuration, // 디버그 라인 표시
//			HitResult,
//			true                   // bIgnoreSelf
//		);
//
//		if (bHit)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
//			HitActors.Add(HitResult.GetActor());
//		}
//	}
//
//	HitTarget = HitResult.GetActor();
//
//	/*
//	FVector CameraForwardVector = GetActorForwardVector();
//	FVector LeftEndVector = CameraForwardVector.RotateAngleAxis(-60.0f, FVector::UpVector);
//	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeToLock;
//	ObjectTypeToLock.Add(EObjectTypeQuery::ObjectTypeQuery3);
//	FCollisionQueryParams QueryParams(NAME_None);
//	TArray<AActor*> ActorsToNotTargeting;
//	ActorsToNotTargeting.Add(this);
//	FVector StartPoint = GetActorLocation();
//	FHitResult HitResult;
//	float ClosestDist = 50.0f;
//	AActor* ClosestHitActor = nullptr;
//	for (int i = 0; i < 120; i += 5)
//	{
//		FVector Direction = LeftEndVector.RotateAngleAxis(i, FVector::UpVector);
//		FVector EndPoint = StartPoint + Direction * TargetingRange;
//		bool bIsHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
//			GetWorld(), StartPoint, EndPoint, 200.f,
//			ObjectTypeToLock, false, ActorsToNotTargeting, EDrawDebugTrace::ForDuration,
//			HitResult, true,
//			FLinearColor::Red, FLinearColor::Green, 2.f);
//		if (bIsHit && HitResult.Distance < ClosestDist)
//		{
//			ClosestDist = HitResult.Distance;
//			ClosestHitActor = HitResult.GetActor();
//		}
//	}
//	if (ClosestHitActor)
//	{
//		bHasLockTarget = true;
//		LockedOnTarget = ClosestHitActor;
//	}
//	*/
//}

void AP4WCharacterBase::FindTarget()
{
	FVector MyLocation = GetActorLocation();
	FVector MyForward = GetActorForwardVector();

	float ViewAngleDegrees = 120.0f; // 전체 시야각 (예: 60도면 좌우 30도씩)
	float ViewRadius = 1000.0f;

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
		nullptr,             // 특정 클래스만 찾고 싶으면 여기에 넣음
		IgnoredActors,
		OutActors
	);

	// 2. Dot Product로 시야각 필터링
	for (AActor* Actor : OutActors)
	{
		if (!Actor) continue;

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

	float BaseActorDist = 500.0f;
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


//DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All)
//{
//}
