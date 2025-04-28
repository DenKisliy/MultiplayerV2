// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameState/MMultiplayerGameState.h"

#include "Kismet/GameplayStatics.h"

#include "../../../Public/GameFramework/MPlayerState.h"
#include "../../../Public/GameFramework/MGameMode.h"
#include "../../../Public/Subsystem/MSessionSubsystem.h"

void AMMultiplayerGameState::StartBeginPlayTimer()
{
	TypeOfTimer = ETypeOfTimer::StartMatch;
	TimerInterval = 1.0f;
	TimerCounter = GetTimeForTimerByType(ETypeOfTimer::StartMatch) + 1;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMMultiplayerGameState::OnTimerCounter, TimerInterval, true);
}

void AMMultiplayerGameState::StartSession_Implementation()
{
	if (UMSessionSubsystem* SessionManager = GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
	{
		SessionManager->StartSession();
	}
}

void AMMultiplayerGameState::OnTimerCounter()
{
	TimerCounter = TimerCounter - 1;

	TimeChecker = TimerCounter;

	SetTimeForPlayers();
}

void AMMultiplayerGameState::OnRep_TimeChecker()
{
	if (TimerCounter != TimeChecker)
	{
		TimerCounter = TimeChecker;

		SetTimeForPlayers();
	}
}

void AMMultiplayerGameState::SetTimeForPlayers()
{
	if (IsValid(GetWorld()))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		if (TimerCounter != -1 && GetPlayerCountFromGameMode() > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMMultiplayerGameState::OnTimerCounter, TimerInterval, true);
		}

		ShowTimeForHUD(true, TimerCounter);

		if (TimerCounter == -1 && TypeOfTimer != ETypeOfTimer::None)
		{
			switch (TypeOfTimer)
			{
			case ETypeOfTimer::StartMatch:
				StartSession();
				GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
				GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
				TimerAccelerationFactorDelegate.BindDynamic(this, &AMMultiplayerGameState::OnTimerAccelerationFactor);
				break;
			}

			TimerFinishDelegate.Broadcast(TypeOfTimer);
			TypeOfTimer = ETypeOfTimer::None;
			TimerInterval = 0;
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
}

void AMMultiplayerGameState::ResurrectionTimer(bool bStart)
{
	if (bStart)
	{
	}
	else
	{
	}
}

void AMMultiplayerGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (PlayerArray.Num() == GetPlayerCountFromGameMode())
	{
		BindDelegateForDeath();

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

	DOREPLIFETIME(AMMultiplayerGameState, TimeChecker);
	//DOREPLIFETIME(AMMultiplayerGameState, AdditionalTimerChecker);
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

void AMMultiplayerGameState::OnTimerAccelerationFactor(float NewTimerPeriod)
{
	if (IsValid(GetWorld()))
	{
		if (NewTimerPeriod == 0)
		{
			TimerCounter = -5;

			TimeChecker = TimerCounter;

			TypeOfTimer = ETypeOfTimer::None;

			TimerInterval = 0;

			SetTimeForPlayers();
		}

		if (NewTimerPeriod != 0 && TimerInterval != NewTimerPeriod)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

			if (TypeOfTimer == ETypeOfTimer::None)
			{
				StartCaptureStationTimer();
			}
			TimerInterval = NewTimerPeriod;

			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMMultiplayerGameState::OnTimerCounter, TimerInterval, true);
		}
	}
}