// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "../../../Multiplayer.h"

#include "MBaseGameState.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FTimerFinish, ETypeOfTimer);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAdditionalTimerFinish, ETypeOfAdditionalTimer, TypeOfAdditionalTimer);
DECLARE_DYNAMIC_DELEGATE_OneParam(FTimerAccelerationFactor, float, NewTimerPeriod);

UCLASS()
class MULTIPLAYER_API AMBaseGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	float MainTimerInterval;

	int MainTimerCounter;

	ETypeOfTimer MainTypeOfTimer = ETypeOfTimer::None;

	int CaptureStationCounter = 0;

public:
	FTimerFinish TimerFinishDelegate;

	FTimerAccelerationFactor TimerAccelerationFactorDelegate;

	FAdditionalTimerFinish AdditionalTimerDelegate;

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetResultOfGame(bool bWin);

protected:
	int GetTimeForTimerByType(ETypeOfTimer Type);

	void StartCaptureStationTimer();

	void ShowTimeForHUD(bool bMain, int Time);

	UFUNCTION(Server, Reliable)
	void SaveResultOfGame(const bool ResultOfGame, const bool bStandalone);

	UFUNCTION(Server, Reliable)
	void ChangePlayersInputStates(bool bUIMode);
};
