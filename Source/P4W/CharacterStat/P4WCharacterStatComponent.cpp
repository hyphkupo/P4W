// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/P4WCharacterStatComponent.h"

// Sets default values for this component's properties
UP4WCharacterStatComponent::UP4WCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UP4WCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UP4WCharacterStatComponent::SetJobStat(int32 InNewLevel)
{

}
