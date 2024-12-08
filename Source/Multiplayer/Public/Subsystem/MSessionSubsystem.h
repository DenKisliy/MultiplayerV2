// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemUtils.h"

#include "Kismet/GameplayStatics.h"

#include "../GameFramework/MGameMode.h"

#include "MSessionSubsystem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResultOfFindSessionsSignature, bool, IsFindSession);

UCLASS()
class MULTIPLAYER_API UMSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	FOnResultOfFindSessionsSignature ResultOfFindSessionsDelegate;

private:
	FString OpenLevelURL = "";

	int CountOfMaxNumPlayers = 0;

	FName GameSessionName = "";

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	TArray<FOnlineSessionSearchResult> ArrayOfFindSessions;

	bool bCreateSession = false;

	bool bJoinToSession = false;

	FString JoinSessionName;

public:
	virtual void Deinitialize() override;

	void CreateSession(FName SessionName, bool bIsLAN, bool bIsDedicatedServer);

	void DestroySession();

	void FindSessions();

	TArray<FOnlineSessionSearchResult> GetFindSessionsNamesArray();

	void StartSession();

	void EndSession();

	bool IsCreateOrStartSession();

	void SetJoinSessionName(FString SessionName);

	void ConnectToSession();

private:
	void SetSessionInfoFromGameMode();

	void JoinToSession(FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void OnFindSessionsComplete(bool bWasSuccessful);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnEndSessionCompleted(FName SessionName, bool bWasSuccessful);

	FOnlineSessionSearchResult GetSessionDataByName();
};
