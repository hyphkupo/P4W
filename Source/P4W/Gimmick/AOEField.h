// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AOEField.generated.h"

UCLASS()
class P4W_API AAOEField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAOEField();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

protected:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void ApplyDamage();

    UPROPERTY(EditAnywhere, Category = "AOE")
    class USphereComponent* CollisionSphere;

    UPROPERTY(EditAnywhere, Category = "AOE")
    float Radius = 300.f;

    UPROPERTY(EditAnywhere, Category = "AOE")
    float Damage = 20.f;

    UPROPERTY(EditAnywhere, Category = "AOE")
    float Duration = 2.0f;

    TSet<AActor*> OverlappingActors;
};
