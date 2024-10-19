// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Kismet/GameplayStatics.h"

#include "../../Multiplayer.h"
#include "GameplayEffect.h"

#include "MGameInstance.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginSignature, bool, IsSuccesdLogin);

USTRUCT(BlueprintType)
struct FPlayerInfoStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;
	
	UPROPERTY(BlueprintReadWrite)
	EResultOfGame ResultOfGame;

	UPROPERTY(BlueprintReadWrite)
	ETypeOfCharacter CharacterType;
	
	UPROPERTY(BlueprintReadWrite)
	bool bCharacterDeath;

	FPlayerInfoStruct()
	{
		PlayerName = "";
		ResultOfGame = EResultOfGame::None;
		CharacterType = ETypeOfCharacter::None;
		bCharacterDeath = false;
	}

	void UpdatePlayerInfo(FPlayerInfoStruct& NewPlayerInfo)
	{
		PlayerName = NewPlayerInfo.PlayerName;
		ResultOfGame = NewPlayerInfo.ResultOfGame;
		CharacterType = NewPlayerInfo.CharacterType;
		bCharacterDeath = NewPlayerInfo.bCharacterDeath;
	}

	void SetPlayerName(FString NewPlayerName)
	{
		PlayerName = NewPlayerName;
	}

	void SetCharacterType(ETypeOfCharacter NewCharacterType)
	{
		CharacterType = NewCharacterType;
	}

	void SetCharacterDeath(bool bValue)
	{
		bCharacterDeath = bValue;
	}
};

UCLASS()
class MULTIPLAYER_API UMGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	FPlayerInfoStruct PlayerInfo;

public:
	UMGameInstance(const FObjectInitializer& ObjectInitializer);

	void SavePlayerInfo(FPlayerInfoStruct& NewPlayerInfo);

	UFUNCTION(BlueprintCallable)
	FPlayerInfoStruct& GetPlayerInfoFromGameInstance();
};
