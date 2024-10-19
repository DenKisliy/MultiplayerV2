// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "MGameMode.h"
#include "MPlayerHUD.h"
#include "MPlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "../Subsystem/MSessionSubsystem.h"
#include "../../Multiplayer.h"

#include "MGameState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerFinish, ETypeOfTimer, TypeOfFinishTimer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdditionalTimerFinish, ETypeOfAdditionalTimer, TypeOfAdditionalTimer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerAccelerationFactor, float, NewTimerPeriod);


UCLASS()
class MULTIPLAYER_API AMGameState : public AGameStateBase
{
	GENERATED_BODY()

private:
	int CountOfDeathPlayers = 0;
	
	//main timer
	float TimerInterval;

	int TimerCounter;

	FTimerHandle TimerHandle;

	UPROPERTY(ReplicatedUsing = OnRep_TimeChecker)
	int TimeChecker;

	ETypeOfTimer TypeOfTimer = ETypeOfTimer::None;

	//additional timer
	ETypeOfAdditionalTimer TypeOfAdditionalTimer = ETypeOfAdditionalTimer::None;

	const int AdditionalTimerInterval = 1;

	int AdditionalTimerCounter;

	FTimerHandle AdditionalTimerHandle;

	UPROPERTY(ReplicatedUsing = OnRep_AdditionalTimerChecker)
	int AdditionalTimerChecker;

public:
	FTimerFinish TimerFinishDelegate;

	FTimerAccelerationFactor TimerAccelerationFactorDelegate;

	FAdditionalTimerFinish AdditionalTimerDelegate;

public:
	void StartBeginPlayTimer();
	
	void StartCaptureStationTimer();

	void StartResurrectionTimer();

	void StopResurrectionTimer();

	UFUNCTION(Server, Reliable)
	void StartSession();

	UFUNCTION(Server, Reliable)
	void DestroySession();

	UFUNCTION(Server, Reliable)
	void SaveResultOfGame(EResultOfGame ResultOfGame);

protected:
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

private:
	int GetPlayerCountFromGameMode();

	int GetTimeForTimerByType();

	int GetAdditionalTimeForTimerByType();

	//main timer
	UFUNCTION()
	void OnPlayerDeath(bool bDeathPlayer);

	UFUNCTION()
	void OnRep_TimeChecker();

	void SetTimeForPlayers();

	UFUNCTION()
	void OnTimerCounter();

	UFUNCTION()
	void OnTimerAccelerationFactor(float NewTimerPeriod);

	// additional timer
	UFUNCTION()
	void OnRep_AdditionalTimerChecker();

	void SetAdditionalTimeForPlayers();
	
	UFUNCTION()
	void OnAdditionalTimerCounter();

	//show time
	void ShowTimeForHUD(bool bMain, int Time);

};
