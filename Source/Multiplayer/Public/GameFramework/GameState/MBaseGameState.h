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
	float TimerInterval;

	int TimerCounter;

	ETypeOfTimer TypeOfTimer = ETypeOfTimer::None;

public:
	FTimerFinish TimerFinishDelegate;

	FTimerAccelerationFactor TimerAccelerationFactorDelegate;

	FAdditionalTimerFinish AdditionalTimerDelegate;

protected:
	int GetTimeForTimerByType(ETypeOfTimer Type);

	void StartCaptureStationTimer();

	void ShowTimeForHUD(bool bMain, int Time);
};
