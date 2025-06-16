// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameState/MMultiplayerGameState.h"

#include "Kismet/GameplayStatics.h"

#include "../../../Public/GameFramework/MPlayerState.h"
#include "../../../Public/GameFramework/MGameMode.h"
#include "../../../Public/Subsystem/MSessionSubsystem.h"
#include "../../../Public/Subsystem/MPlayerInfoSubsystem.h"

void AMMultiplayerGameState::StartBeginPlayTimer()
{
	MainTypeOfTimer = ETypeOfTimer::StartMatch;
	MainTimerInterval = 1.0f;
	MainTimerCounter = GetTimeForTimerByType(ETypeOfTimer::StartMatch) + 1;

	GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &AMMultiplayerGameState::OnTimerCounter, MainTimerInterval, true);
}

void AMMultiplayerGameState::StartSession_Implementation()
{
	if (UMSessionSubsystem* SessionManager = GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
	{
		SessionManager->StartSession();
	}
}

void AMMultiplayerGameState::EndSession_Implementation()
{
	if (UMSessionSubsystem* SessionManager = GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
	{
		SessionManager->EndSession();
	}
}

void AMMultiplayerGameState::OnTimerCounter()
{
	MainTimerCounter = MainTimerCounter - 1;

	MainTimeChecker = MainTimerCounter;

	SetTimeForPlayers();
}

void AMMultiplayerGameState::OnRep_MainTimeChecker()
{
	if (MainTimerCounter != MainTimeChecker)
	{
		MainTimerCounter = MainTimeChecker;

		SetTimeForPlayers();
	}
}

void AMMultiplayerGameState::SetTimeForPlayers()
{
	if (IsValid(GetWorld()))
	{
		GetWorld()->GetTimerManager().ClearTimer(MainTimerHandle);

		if (MainTimerCounter != -1 && GetPlayerCountFromGameMode() > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &AMMultiplayerGameState::OnTimerCounter, MainTimerInterval, true);
		}

		ShowTimeForHUD(true, MainTimerCounter);

		if (MainTimerCounter == -1 && MainTypeOfTimer != ETypeOfTimer::None)
		{
			switch (MainTypeOfTimer)
			{
			case ETypeOfTimer::StartMatch:
				StartSession();
				ChangePlayersInputStates(false);
				TimerAccelerationFactorDelegate.BindDynamic(this, &AMMultiplayerGameState::OnTimerAccelerationFactor);
				break;
			}

			TimerFinishDelegate.Broadcast(MainTypeOfTimer);
			MainTypeOfTimer = ETypeOfTimer::None;
			MainTimerInterval = 0;
			GetWorld()->GetTimerManager().ClearTimer(MainTimerHandle);
		}
	}
}

void AMMultiplayerGameState::ResurrectionTimer(bool bStart)
{
	TypeOfAdditionalTimer = bStart ? ETypeOfAdditionalTimer::ResurrectionPlayer : ETypeOfAdditionalTimer::None;
	AdditionalTimerCounter = bStart ? GetAdditionalTimeForTimerByType() + 1 : -1;

	if (bStart)
	{
		GetWorld()->GetTimerManager().SetTimer(AdditionalTimerHandle, this, &AMMultiplayerGameState::OnAdditionalTimerCounter, AdditionalTimerInterval, true);
	}
	else
	{
		SetAdditionalTimeForPlayers();
	}
}

void AMMultiplayerGameState::SetResultOfGame(bool bWin)
{
	Super::SetResultOfGame(bWin);
	EndSession();
}

void AMMultiplayerGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (PlayerArray.Num() == GetPlayerCountFromGameMode())
	{
		BindDelegatesForPlayers();

		if (UMSessionSubsystem* SessionManager = GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
		{
			if (SessionManager->IsCreateOrStartSession())
			{
				StartBeginPlayTimer();
			}
		}
	}
}

void AMMultiplayerGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMMultiplayerGameState, MainTimeChecker);
	DOREPLIFETIME(AMMultiplayerGameState, AdditionalTimerChecker);
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

int AMMultiplayerGameState::GetAdditionalTimeForTimerByType()
{
	if (IsValid(GetWorld()))
	{
		if (UGameplayStatics::GetGameMode(GetWorld()))
		{
			if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				return *GameMode->AdditionalTimerMap.Find(TypeOfAdditionalTimer);
			}
		}
	}
	return 0;
}

void AMMultiplayerGameState::BindDelegatesForPlayers()
{
	for (APlayerState* BasePS : PlayerArray)
	{
		if (AMPlayerState* CharacterPS = Cast<AMPlayerState>(BasePS))
		{
			CharacterPS->PlayerDeathDelegate.AddDynamic(this, &AMMultiplayerGameState::OnPlayerDeath);
		}
	}
}

void AMMultiplayerGameState::OnPlayerDeath(bool bDeathPlayer)
{
	CountOfDeathPlayers = bDeathPlayer ? CountOfDeathPlayers + 1 : CountOfDeathPlayers - 1 < 0 ? 0 : CountOfDeathPlayers - 1;

	if (CountOfDeathPlayers == GetPlayerCountFromGameMode())
	{
		FTimerDelegate EndTimerDelegate;
		EndTimerDelegate.BindUFunction(this, FName("SetResultOfGame"), 0);

		GetWorldTimerManager().SetTimer(MainTimerHandle, EndTimerDelegate, 2.0f, false);
	}
}

void AMMultiplayerGameState::OnTimerAccelerationFactor(float NewTimerPeriod)
{
	if (IsValid(GetWorld()))
	{
		if (NewTimerPeriod == 0)
		{
			MainTimerCounter = -5;

			MainTimeChecker = MainTimerCounter;

			MainTypeOfTimer = ETypeOfTimer::None;

			MainTimerInterval = 0;

			SetTimeForPlayers();
		}

		if (NewTimerPeriod != 0 && MainTimerInterval != NewTimerPeriod)
		{
			GetWorld()->GetTimerManager().ClearTimer(MainTimerHandle);

			if (MainTypeOfTimer == ETypeOfTimer::None)
			{
				StartCaptureStationTimer();
			}
			MainTimerInterval = NewTimerPeriod;

			GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &AMMultiplayerGameState::OnTimerCounter, MainTimerInterval, true);
		}
	}
}

void AMMultiplayerGameState::OnRep_AdditionalTimerChecker()
{
	if (AdditionalTimerCounter != AdditionalTimerChecker)
	{
		AdditionalTimerCounter = AdditionalTimerChecker;

		SetAdditionalTimeForPlayers();
	}
}

void AMMultiplayerGameState::SetAdditionalTimeForPlayers()
{
	if (IsValid(GetWorld()))
	{
		GetWorld()->GetTimerManager().ClearTimer(AdditionalTimerHandle);

		if (AdditionalTimerCounter != -1 && GetPlayerCountFromGameMode() > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(AdditionalTimerHandle, this, &AMMultiplayerGameState::OnAdditionalTimerCounter, AdditionalTimerInterval, true);
		}

		ShowTimeForHUD(false, AdditionalTimerCounter);

		if (AdditionalTimerCounter == -1 && TypeOfAdditionalTimer != ETypeOfAdditionalTimer::None)
		{
			AdditionalTimerDelegate.ExecuteIfBound(TypeOfAdditionalTimer);
			TypeOfAdditionalTimer = ETypeOfAdditionalTimer::None;
			GetWorld()->GetTimerManager().ClearTimer(AdditionalTimerHandle);
		}
	}
}

void AMMultiplayerGameState::OnAdditionalTimerCounter()
{
	AdditionalTimerCounter = AdditionalTimerCounter - 1;

	AdditionalTimerChecker = AdditionalTimerCounter;

	SetAdditionalTimeForPlayers();
}
