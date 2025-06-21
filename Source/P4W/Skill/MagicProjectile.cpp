// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

AMagicProjectile::AMagicProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    RootComponent = CollisionComponent;
    CollisionComponent->InitSphereRadius(10.f);
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetNotifyRigidBodyCollision(true);

    CollisionComponent->OnComponentHit.AddDynamic(this, &AMagicProjectile::OnHit);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    MovementComponent->InitialSpeed = 800.0f;
    MovementComponent->MaxSpeed = 800.0f;
    MovementComponent->bRotationFollowsVelocity = true;
    MovementComponent->ProjectileGravityScale = 0.f;
    MovementComponent->bIsHomingProjectile = false; // 켜지는 건 LaunchToTarget에서

    TrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
    TrailComponent->SetupAttachment(RootComponent);
    TrailComponent->bAutoActivate = false;

    InitialLifeSpan = 5.f;

    bReplicates = true;
    SetReplicateMovement(true);
}

void AMagicProjectile::BeginPlay()
{
    Super::BeginPlay();

    if (TrailEffect)
    {
        TrailComponent->SetAsset(TrailEffect);
        TrailComponent->Activate(true);
    }
}

void AMagicProjectile::LaunchToTarget(AActor* InTarget)
{
    if (!InTarget || !MovementComponent) return;

    TargetActor = InTarget;

    MovementComponent->Velocity = GetActorForwardVector() * MovementComponent->InitialSpeed;
    MovementComponent->bIsHomingProjectile = true;
    MovementComponent->HomingTargetComponent = TargetActor->GetRootComponent();
    MovementComponent->HomingAccelerationMagnitude = 3000.f;

    MovementComponent->Activate();
}

void AMagicProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        if (ImpactEffect)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                ImpactEffect,
                GetActorLocation(),
                GetActorRotation()
            );
        }

        Destroy();
    }
}