// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicProjectile.generated.h"

UCLASS()
class P4W_API AMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:
    AMagicProjectile();

    UFUNCTION()
    void LaunchToTarget(AActor* InTarget);

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        FVector NormalImpulse, const FHitResult& Hit);

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class USphereComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UProjectileMovementComponent> MovementComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UNiagaraComponent> TrailComponent;

    UPROPERTY(EditDefaultsOnly, Category = "VFX")
    TObjectPtr<class UNiagaraSystem> TrailEffect;

    UPROPERTY(EditDefaultsOnly, Category = "VFX")
    TObjectPtr<class UNiagaraSystem> ImpactEffect;

    UPROPERTY()
    TObjectPtr<class AActor> TargetActor;
};
