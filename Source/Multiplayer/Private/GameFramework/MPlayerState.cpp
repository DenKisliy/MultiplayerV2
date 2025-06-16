// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/MPlayerState.h"
#include "../../Public/Subsystem/MPlayerInfoSubsystem.h"
#include "../../Public/GameFramework/MPlayerController.h"

AMPlayerState::AMPlayerState()
{
	NetUpdateFrequency = 30.0f;
	SetReplicates(true);
}

void AMPlayerState::UpdateUserNameByLogin(FString UserLogin)
{
	FString OldName = GetPlayerName();

	SetPlayerName(UserLogin);
	bUserNameByLogin = true;

	UpdatePlayerNameOnServer(OldName, UserLogin);
}

void AMPlayerState::UpdatePlayerNameOnServer_Implementation(const FString& OldName, const FString& NewName)
{
	SetPlayerName(NewName);
}

void AMPlayerState::SaveResultOfGame_Implementation(const int ResultOfGame, const bool bStandalone)
{
	if (UMPlayerInfoSubsystem* PlayerInfoManager = GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
	{
		PlayerInfoManager->SaveResultOfGame(ResultOfGame, bStandalone);
	}
}

void AMPlayerState::ChangePlayersInputStates_Implementation(const bool bUIMode)
{
	if (IsValid(GetPlayerController()))
	{
		GetPlayerController()->SetShowMouseCursor(bUIMode);
		if(bUIMode)
		{ 
			GetPlayerController()->SetInputMode(FInputModeUIOnly());
		}
		else
		{
			GetPlayerController()->SetInputMode(FInputModeGameOnly());
		}
	}
}



