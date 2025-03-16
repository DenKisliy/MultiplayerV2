// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState/MBaseGameState.h"
#include "MMultiplayerGameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API AMMultiplayerGameState : public AMBaseGameState
{
	GENERATED_BODY()
	
public:
	void ResurrectionTimer(bool bStart);

	void SaveResultOfMatch(EResultOfGame Result);

protected:
	virtual void AddPlayerState(APlayerState* PlayerState) override;

private:
	int GetPlayerCountFromGameMode();

	void BindDelegateForDeath();
};
