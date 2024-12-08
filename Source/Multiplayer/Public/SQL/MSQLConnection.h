// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "../../../DataBaseConnectorC64/include/mysql.h"

#include "MSQLConnection.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMSQLConnection : public UObject
{
	GENERATED_BODY()

public:
	MYSQL* GlobalConnection = nullptr;

public:
	UMSQLConnection(const FObjectInitializer& ObjectInitializer);

	bool CheckConnection();

	static bool CloseConnection(UMSQLConnection* Connection);
};
