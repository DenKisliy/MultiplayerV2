// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/MGameState.h"
#include "GameFramework/MGameMode.h"
#include "GameFramework/HUD/MPlayingHUD.h"
#include "GameFramework/MPlayerState.h"
#include "../../Public/Managers/MBaseManager.h"

void AMGameState::StartBeginPlayTimer()
{
	TypeOfTimer = ETypeOfTimer::StartMatch;
	TimerInterval = 1.0f;
	TimerCounter = GetTimeForTimerByType() + 1;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMGameState::OnTimerCounter, TimerInterval, true);
}

void AMGameState::StartCaptureStationTimer()
{
	TypeOfTimer = ETypeOfTimer::CaptureStation;
	TimerCounter = GetTimeForTimerByType() + 1;
}

void AMGameState::StartResurrectionTimer()
{
	TypeOfAdditionalTimer = ETypeOfAdditionalTimer::ResurrectionPlayer;
	AdditionalTimerCounter = GetAdditionalTimeForTimerByType() + 1;

	GetWorld()->GetTimerManager().SetTimer(AdditionalTimerHandle, this, &AMGameState::OnAdditionalTimerCounter, AdditionalTimerInterval, true);
}

void AMGameState::StopResurrectionTimer()
{
	AdditionalTimerCounter = -1;
}

void AMGameState::OnTimerCounter()
{
	TimerCounter = TimerCounter - 1;

	TimeChecker = TimerCounter;

	SetTimeForPlayers();
}

void AMGameState::StartSession_Implementation()
{
	if (UMSessionSubsystem* SessionManager = GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
	{
		SessionManager->StartSession();
	}
}

void AMGameState::DestroySession_Implementation()
{
	if (UMSessionSubsystem* SessionManager = GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
	{
		SessionManager->DestroySession();
	}
}

void AMGameState::EndSession_Implementation()
{
	if (UMSessionSubsystem* SessionManager = GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
	{
		SessionManager->EndSession();
	}
}

void AMGameState::SaveResultOfGame_Implementation(EResultOfGame ResultOfGame)
{
	for (APlayerState* BasePS : PlayerArray)
	{
		if (AMPlayerState* CharacterPS = Cast<AMPlayerState>(BasePS))
		{
			//CharacterPS->SaveResultOfGame(ResultOfGame);
		}
	}
}

void AMGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (IsStandAloneMode() && IsValid(GetWorld()))
	{
		if (AMPlayerState* CharacterPS = Cast<AMPlayerState>(PlayerState))
		{
			CharacterPS->PlayerDeathDelegate.AddDynamic(this, &AMGameState::OnPlayerDeath);
		}

		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		TimerAccelerationFactorDelegate.AddDynamic(this, &AMGameState::OnTimerAccelerationFactor);

		if (TimerFinishDelegate.IsBound())
		{
			TimerFinishDelegate.Broadcast(TypeOfTimer);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(ManagerTiner);
			GetWorld()->GetTimerManager().SetTimer(ManagerTiner, this, &AMGameState::OnCheckManagerState, 1.0f, false);
		}
	}
	else
	{
		if (PlayerArray.Num() == GetPlayerCountFromGameMode())
		{
			for (APlayerState* BasePS : PlayerArray)
			{
				if (AMPlayerState* CharacterPS = Cast<AMPlayerState>(BasePS))
				{
					CharacterPS->PlayerDeathDelegate.AddDynamic(this, &AMGameState::OnPlayerDeath);
				}
			}

			if (UMSessionSubsystem* SessionManager = GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
			{
				if (SessionManager->IsCreateOrStartSession())
				{
					StartBeginPlayTimer();
				}
			}
		}
	}
}

void AMGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);

	if (PlayerArray.Num() == 1)
	{
		EndSession();
	}
}

void AMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMGameState, TimeChecker);
	DOREPLIFETIME(AMGameState, AdditionalTimerChecker);
}

int AMGameState::GetPlayerCountFromGameMode()
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

bool AMGameState::IsStandAloneMode()
{
	if (IsValid(GetWorld()))
	{
		if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			return GameMode->bStandAloneMode;
		}
	}

	return false;
}

int AMGameState::GetTimeForTimerByType()
{
	if (IsValid(GetWorld()))
	{
		if(UGameplayStatics::GetGameMode(GetWorld()))
		{
			if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				return *GameMode->MainTimerMap.Find(TypeOfTimer);
			}
		}
	}

	return 0;
}

int AMGameState::GetAdditionalTimeForTimerByType()
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

void AMGameState::OnRep_TimeChecker()
{
	if (TimerCounter != TimeChecker)
	{
		TimerCounter = TimeChecker;

		SetTimeForPlayers();
	}
}

void AMGameState::SetTimeForPlayers()
{
	if (IsValid(GetWorld()))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		if (TimerCounter != -1 && GetPlayerCountFromGameMode() > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMGameState::OnTimerCounter, TimerInterval, true);
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
				TimerAccelerationFactorDelegate.AddDynamic(this, &AMGameState::OnTimerAccelerationFactor);
				break;
			}
			TimerFinishDelegate.Broadcast(TypeOfTimer);
			TypeOfTimer = ETypeOfTimer::None;
			TimerInterval = 0;
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
}

void AMGameState::OnTimerAccelerationFactor(float NewTimerPeriod)
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

			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMGameState::OnTimerCounter, TimerInterval, true);
		}
	}
}

void AMGameState::OnPlayerDeath(bool bDeathPlayer)
{
	if (bDeathPlayer)
	{
		CountOfDeathPlayers = CountOfDeathPlayers + 1;
		if (CountOfDeathPlayers == GetPlayerCountFromGameMode())
		{
			SaveResultOfGame(EResultOfGame::Lost);
		}
	}
	else
	{
		CountOfDeathPlayers = CountOfDeathPlayers - 1;
		
		if (CountOfDeathPlayers < 0)
		{
			CountOfDeathPlayers = 0;
		}
	}
}

void AMGameState::OnRep_AdditionalTimerChecker()
{
	if (AdditionalTimerCounter != AdditionalTimerChecker)
	{
		AdditionalTimerCounter = AdditionalTimerChecker;

		SetAdditionalTimeForPlayers();
	}
}

void AMGameState::SetAdditionalTimeForPlayers()
{
	if (IsValid(GetWorld()))
	{
		GetWorld()->GetTimerManager().ClearTimer(AdditionalTimerHandle);

		if (AdditionalTimerCounter != -1 && GetPlayerCountFromGameMode() > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(AdditionalTimerHandle, this, &AMGameState::OnAdditionalTimerCounter, AdditionalTimerInterval, true);
		}

		ShowTimeForHUD(false, AdditionalTimerCounter);

		if (AdditionalTimerCounter == -1 && TypeOfAdditionalTimer != ETypeOfAdditionalTimer::None)
		{
			AdditionalTimerDelegate.Broadcast(TypeOfAdditionalTimer);
			TypeOfAdditionalTimer = ETypeOfAdditionalTimer::None;
			GetWorld()->GetTimerManager().ClearTimer(AdditionalTimerHandle);
		}
	}
}

void AMGameState::OnAdditionalTimerCounter()
{
	AdditionalTimerCounter = AdditionalTimerCounter - 1;

	AdditionalTimerChecker = AdditionalTimerCounter;

	SetAdditionalTimeForPlayers();
}

void AMGameState::ShowTimeForHUD(bool bMain, int Time)
{
	if (IsValid(GetWorld()))
	{
		if (IsValid(GetWorld()->GetFirstPlayerController()))
		{
			if (APlayerController* playerController = GetWorld()->GetFirstPlayerController())
			{
				if (AMPlayingHUD* PlayerHUD = Cast<AMPlayingHUD>(playerController->GetHUD()))
				{
					PlayerHUD->SetTimeTimerWidget(bMain, Time);
				}
			}
		}
	}
}

void AMGameState::OnCheckManagerState()
{
	if (IsValid(GetWorld()))
	{
		TArray<AActor*> FindManagers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMBaseManager::StaticClass(), FindManagers);

		int CountOfReadyManager = 0;

		for (AActor* Manager : FindManagers)
		{
			if (AMBaseManager* FindManager = Cast<AMBaseManager>(Manager))
			{
				if (FindManager->IsFinishBind())
				{
					CountOfReadyManager = CountOfReadyManager + 1;
				}
			}
		}

		if (FindManagers.Num() == CountOfReadyManager)
		{
			TimerFinishDelegate.Broadcast(ETypeOfTimer::StartMatch);
			GetWorld()->GetTimerManager().ClearTimer(ManagerTiner);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(ManagerTiner);
			GetWorld()->GetTimerManager().SetTimer(ManagerTiner, this, &AMGameState::OnCheckManagerState, 1.0f, false);
		}
	}
}
