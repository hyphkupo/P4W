// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/AOEField.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Monster/P4WBoss.h"
#include "Character/P4WCharacterPlayer_PLD.h"

// Sets default values
AAOEField::AAOEField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->InitSphereRadius(Radius);
    CollisionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
    //CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAOEField::OnOverlapBegin);
    
    SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
    SphereMesh->SetupAttachment(RootComponent);
    SphereMesh->SetHiddenInGame(false);

    bReplicates = true;
}

// Called when the game starts or when spawned
void AAOEField::BeginPlay()
{
	Super::BeginPlay();
	
    //DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, FColor::Red, false, Duration);

    FTimerHandle DamageTimerHandle;
    GetWorldTimerManager().SetTimer(
        DamageTimerHandle,
        this,
        &AAOEField::ApplyDamage,
        Duration,
        false
    );
}

// Called every frame
//void AAOEField::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

//void AAOEField::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//    //if (OtherActor && OtherActor != this && OtherActor->IsA<ACharacter>())
//    //{
//    //    OverlappingActors.Add(OtherActor);
//    //}
//}

void AAOEField::ApplyDamage()
{
    TArray<AActor*> CurrentOverlappingActors;
    CollisionSphere->GetOverlappingActors(CurrentOverlappingActors, ACharacter::StaticClass());

    for (AActor* Actor : CurrentOverlappingActors)
    {
        AP4WBoss* BossActor = Cast<AP4WBoss>(Actor);
        if (BossActor)
        {
            continue;
        }

        AP4WCharacterPlayer_PLD* PLDPawn = Cast<AP4WCharacterPlayer_PLD>(Actor);
        if (PLDPawn && PLDPawn->bIsSheltron)
        {
            UGameplayStatics::ApplyDamage(Actor, Damage * 0.85, nullptr, this, nullptr);
        }
        else
        {
            UGameplayStatics::ApplyDamage(Actor, Damage, nullptr, this, nullptr);
        }
    }

    Destroy();

    //FTimerHandle AOESpawnTimeHandle;
    //GetWorld()->GetTimerManager().SetTimer(
    //    AOESpawnTimeHandle,
    //    FTimerDelegate::CreateLambda([&]()
    //        {
    //            Destroy();
    //        }
    //    ), 1.0f, false
    //);
}
