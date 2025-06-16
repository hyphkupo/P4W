// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/AOEField.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
AAOEField::AAOEField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->InitSphereRadius(Radius);
    CollisionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAOEField::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AAOEField::BeginPlay()
{
	Super::BeginPlay();
	
    // AOE 시각화 (디버그)
    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, FColor::Red, false, Duration);

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

void AAOEField::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherActor->IsA<ACharacter>())
    {
        OverlappingActors.Add(OtherActor);
    }
}

void AAOEField::ApplyDamage()
{
    for (AActor* Actor : OverlappingActors)
    {
        UGameplayStatics::ApplyDamage(Actor, Damage, nullptr, this, nullptr);
    }

    Destroy();
}

