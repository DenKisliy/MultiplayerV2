// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "../Data/MPlayerDataStruct.h"
#include "../SQL/MSQLDatabase.h"

#include "Kismet/GameplayStatics.h"

#include "MPlayerInfoSubsystem.generated.h"

/**
 * 
 */

UCLASS()
class MULTIPLAYER_API UMPlayerInfoSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FString LoginOfUser = "";

	UPROPERTY()
	UMSQLDatabase* Database;

public:
	bool IsPlayerRegistered(FString Login);

	bool RegisterPlayerData(FPlayerInfoData* NewPlayerData);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	bool IsUserSignIn();

	int GetCharacterType();

	bool IsUserDataCorrect(FString Login, FString Password);

private:
	void SetDataBase();

	void CloseDataBase();
};
