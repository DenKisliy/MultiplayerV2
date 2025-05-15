// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState/MBaseGameState.h"

#include "Net/UnrealNetwork.h"

#include "MMultiplayerGameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API AMMultiplayerGameState : public AMBaseGameState
{
	GENERATED_BODY()
	
private:
	//main timer
	float TimerInterval;

	FTimerHandle TimerHandle;

	UPROPERTY(ReplicatedUsing = OnRep_TimeChecker)
	int TimeChecker;

public:
	void ResurrectionTimer(bool bStart);

	virtual void SetResultOfGame(bool bWin) override;

protected:
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

private:
	int GetPlayerCountFromGameMode();

	void BindDelegatesForPlayers();

	void StartBeginPlayTimer();

	UFUNCTION()
	void OnRep_TimeChecker();

	void SetTimeForPlayers();

	UFUNCTION()
	void OnTimerCounter();

	UFUNCTION(Server, Reliable)
	void StartSession();

	UFUNCTION(Server, Reliable)
	void EndSession();

	UFUNCTION()
	void OnTimerAccelerationFactor(float NewTimerPeriod);

	UFUNCTION(Server, Reliable)
	void SaveResultOfGame(const bool ResultOfGame);
};
