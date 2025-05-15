// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState/MBaseGameState.h"
#include "MStandaloneGameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API AMStandaloneGameState : public AMBaseGameState
{
	GENERATED_BODY()
	
private:
	//Manager Timer
	FTimerHandle ManagerTiner;
	
	//main timer
	float TimerInterval;

	FTimerHandle TimerHandle;

public:
	virtual void SetResultOfGame(bool bWin) override;

protected:
	virtual void AddPlayerState(APlayerState* PlayerState) override;

private:
	UFUNCTION()
	void OnCheckManagerState();

	UFUNCTION()
	void OnTimerAccelerationFactor(float NewTimerPeriod);

	void OnTimerCounter();
};
