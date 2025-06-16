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
	int CountOfDeathPlayers = 0;

	//main timer
	float MainTimerInterval;

	FTimerHandle MainTimerHandle;

	UPROPERTY(ReplicatedUsing = OnRep_MainTimeChecker)
	int MainTimeChecker;

	//additional timer
	ETypeOfAdditionalTimer TypeOfAdditionalTimer = ETypeOfAdditionalTimer::None;

	const int AdditionalTimerInterval = 1;

	int AdditionalTimerCounter;

	FTimerHandle AdditionalTimerHandle;

	UPROPERTY(ReplicatedUsing = OnRep_AdditionalTimerChecker)
	int AdditionalTimerChecker;

public:
	void ResurrectionTimer(bool bStart);

	virtual void SetResultOfGame(bool bWin) override;

protected:
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

private:
	int GetPlayerCountFromGameMode();

	int GetAdditionalTimeForTimerByType();

	void BindDelegatesForPlayers();

	UFUNCTION()
	void OnPlayerDeath(bool bDeathPlayer);

	void StartBeginPlayTimer();

	UFUNCTION()
	void OnRep_MainTimeChecker();

	void SetTimeForPlayers();

	UFUNCTION()
	void OnTimerCounter();

	UFUNCTION(Server, Reliable)
	void StartSession();

	UFUNCTION(Server, Reliable)
	void EndSession();

	UFUNCTION()
	void OnTimerAccelerationFactor(float NewTimerPeriod);

	// additional timer
	UFUNCTION()
	void OnRep_AdditionalTimerChecker();

	void SetAdditionalTimeForPlayers();

	UFUNCTION()
	void OnAdditionalTimerCounter();
};
