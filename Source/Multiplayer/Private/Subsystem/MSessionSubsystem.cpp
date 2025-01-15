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
	if (OpenLevelURL.IsEmpty())
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
	if (IOnlineSubsystem* onlineSub = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineSessionPtr sessionsPtrRef = onlineSub->GetSessionInterface())
		{
			sessionsPtrRef->OnStartSessionCompleteDelegates.AddUObject(this, &UMSessionSubsystem::OnStartSessionComplete);
			sessionsPtrRef->StartSession(GameSessionName);
		}
	}
}

void UMSessionSubsystem::EndSession()
{
	if (IOnlineSubsystem* onlineSub = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineSessionPtr sessionsPtrRef = onlineSub->GetSessionInterface())
		{
			sessionsPtrRef->OnEndSessionCompleteDelegates.AddUObject(this, &UMSessionSubsystem::OnEndSessionCompleted);
			sessionsPtrRef->EndSession(GameSessionName);
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
	if (IOnlineSubsystem* onlineSub = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineSessionPtr sessionsPtrRef = onlineSub->GetSessionInterface())
		{
			SetSessionInfoFromGameMode();

			FOnlineSessionSettings sessionSettings;
			sessionSettings.bIsDedicated = bIsDedicatedServer;
			sessionSettings.bAllowInvites = true;
			sessionSettings.bIsLANMatch = bIsLAN;
			sessionSettings.NumPublicConnections = CountOfMaxNumPlayers > 0 ? CountOfMaxNumPlayers : 2;
			sessionSettings.bUseLobbiesIfAvailable = false;
			sessionSettings.bUsesPresence = false;
			sessionSettings.bShouldAdvertise = true;
			sessionSettings.Set(FName("SESSION_NAME_KEY"), SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

			sessionsPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UMSessionSubsystem::OnCreateSessionComplete);

			const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

			sessionsPtrRef->CreateSession(*localPlayer->GetPreferredUniqueNetId(), SessionName, sessionSettings);
		}
	}
}

void UMSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (IOnlineSubsystem* onlineSub = Online::GetSubsystem(GetWorld()))
		{
			if (IOnlineSessionPtr sessionsPtrRef = onlineSub->GetSessionInterface())
			{
				GameSessionName = SessionName;
				bCreateSession = true;
				if (UMPlayerInfoSubsystem* PlayerInfo = GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
				{
					if (PlayerInfo->CreateChatTable(new FCreateTableStructData("Chat_" + SessionName.ToString())))
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "CreateChatTable");
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "CreateChatTable false");
					}
				}
				sessionsPtrRef->OnCreateSessionCompleteDelegates.Clear();
				GetWorld()->ServerTravel(OpenLevelURL.Contains("?listen") ? OpenLevelURL : OpenLevelURL + "?listen");
			}
		}
	}

}

void UMSessionSubsystem::DestroySession()
{
	if (!GameSessionName.ToString().IsEmpty())
	{
		if (IOnlineSubsystem* onlineSub = Online::GetSubsystem(GetWorld()))
		{
			if (IOnlineSessionPtr sessionsPtrRef = onlineSub->GetSessionInterface())
			{
				sessionsPtrRef->OnDestroySessionCompleteDelegates.AddUObject(this, &UMSessionSubsystem::OnDestroySessionComplete);
				if (!GameSessionName.ToString().IsEmpty() && GameSessionName != "None")
				{
					sessionsPtrRef->DestroySession(GameSessionName);
				}
			}
		}
	}
}

void UMSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (IOnlineSubsystem* onlineSub = Online::GetSubsystem(GetWorld()))
		{
			if (IOnlineSessionPtr sessionsPtrRef = onlineSub->GetSessionInterface())
			{
				sessionsPtrRef->OnDestroySessionCompleteDelegates.Clear();
				GameSessionName = "";
				bJoinToSession = false;
				bCreateSession = false;
				UGameplayStatics::OpenLevel(GetWorld(), "MainMenuMap", true);
			}
		}
	}
}

void UMSessionSubsystem::FindSessions()
{
	if (IOnlineSubsystem* onlineSub = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineSessionPtr sessionsPtrRef = onlineSub->GetSessionInterface())
		{
			SessionSearch.Reset();
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, false, EOnlineComparisonOp::Equals);
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->bIsLanQuery = true;

			sessionsPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UMSessionSubsystem::OnFindSessionsComplete);

			const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

			sessionsPtrRef->FindSessions(*localPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
		}
	}
}

void UMSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	ArrayOfFindSessions.Empty();

	if (bWasSuccessful)
	{
		if (IOnlineSubsystem* onlineSub = Online::GetSubsystem(GetWorld()))
		{
			if (IOnlineSessionPtr sessionsPtrRef = onlineSub->GetSessionInterface())
			{
				sessionsPtrRef->OnFindSessionsCompleteDelegates.Clear();
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

void UMSessionSubsystem::JoinToSession(FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	if (SearchResult.IsValid())
	{
		if (IOnlineSubsystem* onlineSub = Online::GetSubsystem(GetWorld()))
		{
			if (IOnlineSessionPtr sessionsPtrRef = onlineSub->GetSessionInterface())
			{
				sessionsPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UMSessionSubsystem::OnJoinSessionComplete);

				const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
				sessionsPtrRef->JoinSession(*localPlayer->GetPreferredUniqueNetId(), SessionName, SearchResult);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "IOnlineSubsystem* onlineSub = Online::GetSubsystem(GetWorld())");
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "IOnlineSessionPtr sessionsPtrRef = onlineSub->GetSessionInterface()");
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "JoinToSession");
	}
}

void UMSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
			{
				if (IOnlineSessionPtr SessionsPtrRef = OnlineSub->GetSessionInterface())
				{
					FString joinAdress = "";
					if (SessionsPtrRef->GetResolvedConnectString(SessionName, joinAdress))
					{

						if (!joinAdress.IsEmpty())
						{
							if (UMPlayerInfoSubsystem* PlayerInfo = GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
							{
								PlayerInfo->SetChatTableName("Chat_" + SessionName.ToString());
							}

							GameSessionName = SessionName;
							bJoinToSession = true;
							SessionsPtrRef->OnJoinSessionCompleteDelegates.Clear();
							PlayerController->ClientTravel(joinAdress, ETravelType::TRAVEL_Absolute);
						}
					}
				}
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "OnJoinSessionComplete");
	}
}

void UMSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (IOnlineSubsystem* onlineSub = Online::GetSubsystem(GetWorld()))
		{
			if (IOnlineSessionPtr sessionsPtrRef = onlineSub->GetSessionInterface())
			{
				sessionsPtrRef->OnStartSessionCompleteDelegates.Clear();
			}
		}
	}
}

void UMSessionSubsystem::OnEndSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (IOnlineSubsystem* onlineSub = Online::GetSubsystem(GetWorld()))
		{
			if (IOnlineSessionPtr sessionsPtrRef = onlineSub->GetSessionInterface())
			{
				sessionsPtrRef->OnEndSessionCompleteDelegates.Clear();
				if (!GameSessionName.ToString().IsEmpty())
				{
					DestroySession();
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
