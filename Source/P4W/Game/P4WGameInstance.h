// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "P4WGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class P4W_API UP4WGameInstance : public UGameInstance
{
	GENERATED_BODY()

	//virtual bool CreateSession(int32 HostingPlayerNum, FName SessionName, const FOnlineSessionSettings& NewSessionSettings) = 0;
	
public:
	// 전역 변수
	UPROPERTY(BlueprintReadWrite)
	float MaxEnmity = 0.0f;

public:
	UP4WGameInstance();

protected:
	//void CreateSession();
	virtual void Init() override;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void CreateSession();
	void SearchSession();
	void ConnectToSession();
	void OnCreateSessionComplete(FName ServerName, bool Succeeded);

	UFUNCTION(BlueprintCallable)
	void CreateServer();

	UFUNCTION(BlueprintCallable)
	void JoinServer();
	
};
