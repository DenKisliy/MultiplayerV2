// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameState/MMultiplayerGameState.h"

#include "Kismet/GameplayStatics.h"

#include "../../../Public/GameFramework/MPlayerState.h"
#include "../../../Public/GameFramework/MGameMode.h"


void AMMultiplayerGameState::ResurrectionTimer(bool bStart)
{
	if (bStart)
	{
	}
	else
	{
	}
}

void AMMultiplayerGameState::SaveResultOfMatch(EResultOfGame Result)
{
}

void AMMultiplayerGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (PlayerArray.Num() == GetPlayerCountFromGameMode())
	{
		BindDelegateForDeath();
	}
}

int AMMultiplayerGameState::GetPlayerCountFromGameMode()
{
	if (IsValid(GetWorld()))
	{
		if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			return GameMode->CountOfMaxNumPlayers;
		}
	}

	return 0;
}

void AMMultiplayerGameState::BindDelegateForDeath()
{
	for (APlayerState* BasePS : PlayerArray)
	{
		if (AMPlayerState* CharacterPS = Cast<AMPlayerState>(BasePS))
		{
			//CharacterPS->PlayerDeathDelegate.AddDynamic(this, &AMGameState::OnPlayerDeath);
		}
	}
}
