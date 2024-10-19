// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "MGameInstance.h"
#include "../Subsystem/MSessionSubsystem.h"
#include "../../Multiplayer.h"

#include "MPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerDeath, bool, bDeathPlayer);

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
	FPlayerInfoStruct PlayerInfo;

	bool bPlayerInSaveZone = false;

	bool bPlayerDeath = false;

public:
	AMPlayerState();

	void SaveNewPlayerName(FString NewPlayerName);

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerState();

	UFUNCTION(Client, Reliable)
	void SaveResultOfGame(EResultOfGame ResultOfGame);

	void SetCharacterInfo(ETypeOfCharacter Type);

	void SaveDataInGameInstance();

	UFUNCTION(BlueprintCallable)
	ETypeOfCharacter GetCharacterType();

	bool IsPlayerInSaveZone();

	void SetPlayerInSaveZone(bool bValue);

	FPlayerInfoStruct& GetPlayerInfo();
};
