// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P4W_API USkillSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	// 해당 키 누르면 바인딩 된 함수 실행되면서 스킬 실행
	// 스킬 시전이 즉발일 수도, 캐스팅일 수도
	// 스킬 시전 끝나면 스킬에 맞는 대미지 줌
	// 도트뎀이라면 지속적으로 대미지
	// 
	// 스킬 끝나면 대미지 원상복귀
};
