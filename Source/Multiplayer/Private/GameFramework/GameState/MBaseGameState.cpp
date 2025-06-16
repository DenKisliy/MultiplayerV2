// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameState/MBaseGameState.h"
#include "../../../Public/GameFramework/MGameMode.h"
#include "../../../Public/GameFramework/HUD/MPlayingHUD.h"
#include "../../../Public/GameFramework/MPlayerState.h"
#include "Kismet/GameplayStatics.h"

void AMBaseGameState::StartCaptureStationTimer()
{
	MainTimerInterval = 1;
	MainTypeOfTimer = ETypeOfTimer::CaptureStation;
	MainTimerCounter = GetTimeForTimerByType(ETypeOfTimer::CaptureStation) + 1;
}

void AMBaseGameState::SetResultOfGame(bool bWin)
{
	if (IsValid(GetWorld()))
	{
		if (UGameplayStatics::GetGameMode(GetWorld()))
		{
			if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				SaveResultOfGame(bWin, GameMode->IsStandAloneMode());
			}
		}
	}
}

int AMBaseGameState::GetTimeForTimerByType(ETypeOfTimer Type)
{
	if (IsValid(GetWorld()))
	{
		if (UGameplayStatics::GetGameMode(GetWorld()))
		{
			if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				return *GameMode->MainTimerMap.Find(Type);
			}
		}
	}

	return 0;
}

void AMBaseGameState::ShowTimeForHUD(bool bMain, int Time)
{
	if (IsValid(GetWorld()))
	{
		if (IsValid(GetWorld()->GetFirstPlayerController()))
		{
			if (AMPlayingHUD* PlayerHUD = Cast<AMPlayingHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
			{
				PlayerHUD->SetTimeTimerWidget(bMain, Time);
			}
		}
	}
}

void AMBaseGameState::SaveResultOfGame_Implementation(const bool ResultOfGame, const bool bStandalone)
{
	for (APlayerState* BasePS : PlayerArray)
	{
		if (AMPlayerState* CharacterPS = Cast<AMPlayerState>(BasePS))
		{
			CharacterPS->SaveResultOfGame(ResultOfGame, bStandalone);
		}
	}
}

void AMBaseGameState::ChangePlayersInputStates_Implementation(bool bUIMode)
{
	for (APlayerState* BasePS : PlayerArray)
	{
		if (AMPlayerState* CharacterPS = Cast<AMPlayerState>(BasePS))
		{
			CharacterPS->ChangePlayersInputStates(bUIMode);
		}
	}
}
