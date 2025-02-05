// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MSessionSubsystem.h"
#include "../../Public/GameFramework/MGameMode.h"
#include "../../Public/Subsystem/MPlayerInfoSubsystem.h"

void UMSessionSubsystem::Deinitialize()
{
	DestroySession();

	Super::Deinitialize();
}

void UMSessionSubsystem::SetSessionInfoFromGameMode()
{
	if (OpenLevelURL.IsEmpty() && IsValid(GetWorld()))
	{
		if (UGameplayStatics::GetGameMode(GetWorld()))
		{
			if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				OpenLevelURL = GameMode->SessionLevelURL;
				CountOfMaxNumPlayers = GameMode->CountOfMaxNumPlayers;
			}
		}
	}
}

TArray<FOnlineSessionSearchResult> UMSessionSubsystem::GetFindSessionsArray()
{
	return ArrayOfFindSessions;
}

void UMSessionSubsystem::StartSession()
{
	if (IsValid(GetWorld()))
	{
		if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
		{
			if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
			{
				SessionsPtrRef->OnStartSessionCompleteDelegates.AddUObject(this, &UMSessionSubsystem::OnStartSessionComplete);
				SessionsPtrRef->StartSession(GameSessionName);
			}
		}
	}
}

void UMSessionSubsystem::EndSession()
{
	if (IsValid(GetWorld()))
	{
		if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
		{
			if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
			{
				SessionsPtrRef->OnEndSessionCompleteDelegates.AddUObject(this, &UMSessionSubsystem::OnEndSessionCompleted);
				SessionsPtrRef->EndSession(GameSessionName);
			}
		}
	}
}

bool UMSessionSubsystem::IsCreateOrStartSession()
{
	return bCreateSession || bJoinToSession;
}

void UMSessionSubsystem::ConnectToSession(FString JoinSessionName)
{
	if (!JoinSessionName.IsEmpty())
	{
		JoinToSession(FName(*JoinSessionName), GetSessionDataByName(JoinSessionName));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "JoinSessionName empty");
	}
}

void UMSessionSubsystem::CreateSession(FName SessionName, bool bIsLAN, bool bIsDedicatedServer)
{
	if (IsValid(GetWorld()))
	{
		if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
		{
			if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
			{
				SetSessionInfoFromGameMode();

				FOnlineSessionSettings SessionSettings;
				SessionSettings.bIsDedicated = bIsDedicatedServer;
				SessionSettings.bAllowInvites = true;
				SessionSettings.bIsLANMatch = bIsLAN;
				SessionSettings.NumPublicConnections = CountOfMaxNumPlayers > 0 ? CountOfMaxNumPlayers : 2;
				SessionSettings.bUseLobbiesIfAvailable = false;
				SessionSettings.bUsesPresence = false;
				SessionSettings.bShouldAdvertise = true;
				SessionSettings.Set(FName("SESSION_NAME_KEY"), SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

				SessionsPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UMSessionSubsystem::OnCreateSessionComplete);

				const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

				SessionsPtrRef->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), SessionName, SessionSettings);
			}
		}
	}
}

void UMSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (IsValid(GetWorld()))
		{
			if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
			{
				if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
				{
					GameSessionName = SessionName;
					bCreateSession = true;
					if (UMPlayerInfoSubsystem* PlayerInfo = GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
					{
						PlayerInfo->CreateChatTable(new FCreateTableStructData("Chat_" + SessionName.ToString()));
					}

					SessionsPtrRef->OnCreateSessionCompleteDelegates.Clear();
					GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
					GetWorld()->ServerTravel(OpenLevelURL.Contains("?listen") ? OpenLevelURL : OpenLevelURL + "?listen");
				}
			}
		}
	}
}

void UMSessionSubsystem::DestroySession()
{
	if (!GameSessionName.ToString().IsEmpty())
	{
		if (IsValid(GetWorld()))
		{
			if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
			{
				if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
				{
					SessionsPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this, &UMSessionSubsystem::OnDestroySessionComplete);
					if (!GameSessionName.ToString().IsEmpty() && GameSessionName != "None")
					{
						SessionsPtrRef->DestroySession(GameSessionName);
					}
				}
			}
		}
	}
}

void UMSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (IsValid(GetWorld()))
		{
			if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
			{
				if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
				{
					SessionsPtrRef->OnDestroySessionCompleteDelegates.Clear();
					GameSessionName = "";
					bJoinToSession = false;
					bCreateSession = false;
					UGameplayStatics::OpenLevel(GetWorld(), "MainMenuMap", true);
				}
			}
		}
	}
}

void UMSessionSubsystem::FindSessions()
{
	if (IsValid(GetWorld()))
	{
		if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
		{
			if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
			{
				SessionSearch.Reset();
				SessionSearch = MakeShareable(new FOnlineSessionSearch());
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, false, EOnlineComparisonOp::Equals);
				SessionSearch->MaxSearchResults = 20;
				SessionSearch->bIsLanQuery = true;

				SessionsPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UMSessionSubsystem::OnFindSessionsComplete);

				const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

				SessionsPtrRef->FindSessions(*localPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
			}
		}
	}
}

void UMSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	ArrayOfFindSessions.Empty();

	if (bWasSuccessful)
	{
		if (IsValid(GetWorld()))
		{
			if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
			{
				if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
				{
					SessionsPtrRef->OnFindSessionsCompleteDelegates.Clear();
					for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
					{
						if (Result.IsValid())
						{
							ArrayOfFindSessions.Add(Result);
						}
					}

					ResultOfFindSessionsDelegate.ExecuteIfBound(ArrayOfFindSessions);
				}
			}
		}
	}
}

void UMSessionSubsystem::JoinToSession(FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	if (SearchResult.IsValid())
	{
		if (IsValid(GetWorld()))
		{
			if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
			{
				if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
				{
					SessionsPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UMSessionSubsystem::OnJoinSessionComplete);

					const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
					SessionsPtrRef->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), SessionName, SearchResult);
				}
			}
		}
	}
}

void UMSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (IsValid(GetWorld()))
		{
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
				{
					if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
					{
						FString JoinAdress = "";
						if (SessionsPtrRef->GetResolvedConnectString(SessionName, JoinAdress))
						{
							if (!JoinAdress.IsEmpty())
							{
								if (UMPlayerInfoSubsystem* PlayerInfo = GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
								{
									PlayerInfo->SetChatTableName("Chat_" + SessionName.ToString());
								}

								GameSessionName = SessionName;
								bJoinToSession = true;
								SessionsPtrRef->OnJoinSessionCompleteDelegates.Clear();
								GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
								PlayerController->ClientTravel(JoinAdress, ETravelType::TRAVEL_Absolute);
							}
						}
					}
				}
			}
		}
	}
}

void UMSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (IsValid(GetWorld()))
		{
			if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
			{
				if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
				{
					SessionsPtrRef->OnStartSessionCompleteDelegates.Clear();
				}
			}
		}
	}
}

void UMSessionSubsystem::OnEndSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (IsValid(GetWorld()))
		{
			if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
			{
				if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
				{
					SessionsPtrRef->OnEndSessionCompleteDelegates.Clear();
					if (!GameSessionName.ToString().IsEmpty())
					{
						DestroySession();
					}
				}
			}
		}
	}
}

FOnlineSessionSearchResult UMSessionSubsystem::GetSessionDataByName(FString SessionName)
{
	for (FOnlineSessionSearchResult SessionData : GetFindSessionsArray())
	{
		FString FindSessionName = "";
		SessionData.Session.SessionSettings.Get(FName("SESSION_NAME_KEY"), FindSessionName);
		if (FindSessionName == SessionName && !FindSessionName.IsEmpty())
		{
			return SessionData;
		}
	}

	return FOnlineSessionSearchResult();
}
