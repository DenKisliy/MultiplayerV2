// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "../Data/MPlayerDataStruct.h"

#include "MPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API AMPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	FPlayerDeath PlayerDeathDelegate;

private:
	bool bInSaveZone = false;

	bool bPlayerDeath = false;

	bool bUserNameByLogin = false;

public:
	AMPlayerState();

	bool IsInSaveZone() { return bInSaveZone; }

	void SetPlayerInSaveZone(bool bValue) { bInSaveZone = bValue; }

	bool IsUserNameByLogin() { return bUserNameByLogin; }

	void UpdateUserNameByLogin(FString UserLogin);

};
