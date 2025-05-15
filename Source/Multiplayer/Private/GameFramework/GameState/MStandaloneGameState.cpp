// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameState/MStandaloneGameState.h"
#include "../../../Public/Managers/MBaseManager.h"
#include "Kismet/GameplayStatics.h"

void AMStandaloneGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (IsValid(GetWorld()))
	{
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		TimerAccelerationFactorDelegate.BindDynamic(this, &AMStandaloneGameState::OnTimerAccelerationFactor);

		if (TimerFinishDelegate.IsBound())
		{
			TimerFinishDelegate.Broadcast(TypeOfTimer);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(ManagerTiner);
			GetWorld()->GetTimerManager().SetTimer(ManagerTiner, this, &AMStandaloneGameState::OnCheckManagerState, 1.0f, false);
		}
	}
}

void AMStandaloneGameState::OnCheckManagerState()
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

		if (FindManagers.Num() == CountOfReadyManager && TimerFinishDelegate.IsBound())
		{
			TimerFinishDelegate.Broadcast(ETypeOfTimer::StartMatch);
			GetWorld()->GetTimerManager().ClearTimer(ManagerTiner);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(ManagerTiner);
			GetWorld()->GetTimerManager().SetTimer(ManagerTiner, this, &AMStandaloneGameState::OnCheckManagerState, 1.0f, false);
		}
	}
}

void AMStandaloneGameState::OnTimerAccelerationFactor(float NewTimerPeriod)
{
	if (IsValid(GetWorld()))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		if (NewTimerPeriod == 0)
		{
			TimerCounter = -1;
			TimerInterval = 0;

			TypeOfTimer = ETypeOfTimer::None;

			ShowTimeForHUD(true, TimerCounter);
		}

		if (NewTimerPeriod != 0 && TimerInterval != NewTimerPeriod)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

			if (TypeOfTimer == ETypeOfTimer::None)
			{
				StartCaptureStationTimer();
			}

			TimerInterval = NewTimerPeriod;

			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMStandaloneGameState::OnTimerCounter, TimerInterval, true);
		}
	}
}

void AMStandaloneGameState::OnTimerCounter()
{
	TimerCounter = TimerCounter - 1;

	ShowTimeForHUD(true, TimerCounter);

	if (TimerCounter == -1 && TypeOfTimer != ETypeOfTimer::None)
	{
		TimerFinishDelegate.Broadcast(TypeOfTimer);
		TypeOfTimer = ETypeOfTimer::None;
		TimerInterval = 0;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void AMStandaloneGameState::SetResultOfGame(bool bWin)
{
}
