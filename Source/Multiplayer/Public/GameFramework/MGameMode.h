// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "../../Multiplayer.h"
#include "../Managers/MSpawnItemManager.h"
#include "../Data/MPlayerDataStruct.h"

#include "Engine/DataTable.h"

#include "MGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API AMGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	bool bStandAloneMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	FString StandAloneLevelURL = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Varible")
	FString SessionLevelURL = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Varible")
	int CountOfMaxNumPlayers = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer Values")
	TMap<ETypeOfTimer, int> MainTimerMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer Values")
	TMap<ETypeOfAdditionalTimer, int> AdditionalTimerMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Info")
	TMap<ETypeOfCharacter, TSubclassOf<AActor>> PlayersClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Manager)
	TSubclassOf<AActor> CaptureBaseManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Manager)
	TSubclassOf<AActor> SpawnBotManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Manager)
	TSubclassOf<AActor> ItemManagerClass;

private:
	AActor* ItemManager;

public:
	AMGameMode();

	void SetSpawnItems(FItemTypeInfo ItemType, FVector Location);

	bool IsStandAloneMode();

protected:
	virtual void BeginPlay() override;
};
