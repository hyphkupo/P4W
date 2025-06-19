// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/P4WGameInstance.h"
#include "Kismet/GameplayStatics.h"

UP4WGameInstance::UP4WGameInstance()
{
}

void UP4WGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionInterface->CreateSession(0, TEXT("SESSION NAME"), SessionSettings);
	}
}

void UP4WGameInstance::SearchSession()
{
	IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();
	if (OnlineSubsystemInterface)
	{
		SessionInterface = OnlineSubsystemInterface->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FOnlineSessionSettings SessionSettings;
			SessionInterface->CreateSession(0, TEXT("SessionName1"), SessionSettings);
			//if (/*Success*/)
			//{
				UGameplayStatics::OpenLevel(this, TEXT("Lobby"), true);
			//}
		}
	}
}

void UP4WGameInstance::ConnectToSession()
{
}

void UP4WGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnOnCreateSessionComplete, Succeeded: %d"), Succeeded);
	//if (Succeeded)
	//{
	//	GetWorld()->ServerTravel()
	//}
}

void UP4WGameInstance::CreateServer()
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;

	SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);
}

void UP4WGameInstance::JoinServer()
{
}

void UP4WGameInstance::Init()
{
	IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();
	if (OnlineSubsystemInterface)
	{
		SessionInterface = OnlineSubsystemInterface->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UP4WGameInstance::OnCreateSessionComplete);
			//SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UP4WGameInstance::OnDestroySessionComplete);
			//SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UP4WGameInstance::OnFindSessionsComplete);
			//SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UP4WGameInstance::OnJoinSessionComplete);
		}
	}
}
