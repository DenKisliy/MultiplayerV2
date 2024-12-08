// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include <string>
#include "../Data/MSQLDataStruct.h"

#include "../../../DataBaseConnectorC64/include/mysql.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"
#include "Engine/NetConnection.h"

#include "MSQLDatabase.generated.h"

/**
 * 
 */

using namespace std;

UCLASS()
class MULTIPLAYER_API UMSQLDatabase : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UMSQLConnection* DatabaseConnection;

public:
	UMSQLDatabase(const FObjectInitializer& ObjectInitializer);

	bool InitConnection();

	bool IsCloseConnection();

	bool ExecuteQuery(FString Query);

	FQueryResult GetQueryData(const FString& Query);

private:
	MSQLConnectorQueryResult* RunQueryAndGetResults(FString Query);

	FDatabaseData* GetDefaultSettings();
};
