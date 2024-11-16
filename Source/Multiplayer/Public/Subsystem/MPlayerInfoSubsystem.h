// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "../Data/MPlayerDataStruct.h"

#include "Kismet/GameplayStatics.h"
#include "SQLiteDatabase.h"

#include "MPlayerInfoSubsystem.generated.h"

/**
 * 
 */


class FSQLiteDatabase;
class FSQLitePreparedStatement;

UCLASS()
class MULTIPLAYER_API UMPlayerInfoSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	FSQLiteDatabase* MultiplayerDb;

public:
	bool IsPlayerRegistered(FString Login);

	bool IsLoginPasswordCorrect(FString Login, FString Password);

	bool RegisterPlayerData(FPlayerInfoData* NewPlayerData);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

private:
	FPlayerInfoData* GetPlayerInfo(FString Login);

	void SetDataBase();

	void CloseDataBase();
};
