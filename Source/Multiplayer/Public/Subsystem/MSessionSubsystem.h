// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Kismet/GameplayStatics.h"

#include "MSessionSubsystem.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnResultOfFindSessionsSignature, TArray<FOnlineSessionSearchResult>);

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

public:
	virtual void Deinitialize() override;

	void CreateSession(FName SessionName, bool bIsLAN, bool bIsDedicatedServer);

	void DestroySession();

	void FindSessions();

	TArray<FOnlineSessionSearchResult> GetFindSessionsArray();

	void StartSession();

	void EndSession();

	bool IsCreateOrStartSession();

	void ConnectToSession(FString JoinSessionName);

private:
	void SetSessionInfoFromGameMode();

	void JoinToSession(FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void OnFindSessionsComplete(bool bWasSuccessful);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnEndSessionCompleted(FName SessionName, bool bWasSuccessful);

	FOnlineSessionSearchResult GetSessionDataByName(FString SessionName);
};
