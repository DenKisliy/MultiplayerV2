// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameState/MBaseGameState.h"
#include "../../../Public/GameFramework/MGameMode.h"
#include "../../../Public/GameFramework/HUD/MPlayingHUD.h"
#include "Kismet/GameplayStatics.h"

void AMBaseGameState::StartCaptureStationTimer()
{
	TimerInterval = 1;
	TypeOfTimer = ETypeOfTimer::CaptureStation;
	TimerCounter = GetTimeForTimerByType(ETypeOfTimer::CaptureStation) + 1;
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
