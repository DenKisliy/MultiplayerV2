#pragma once

#include "CoreMinimal.h"
#include <string>

#include "../SQL/MSQLConnection.h"

#include "MSQLDataStruct.generated.h"

UENUM(BlueprintType)
enum class ECreatTableFieldType : uint8
{
	Int,
	Varchar,
	Text,
	Datetime,
};

UENUM(BlueprintType)
enum class EQueryResultValType : uint8
{
	Int,
	Varchar,
	UnsupportedValueType
};

USTRUCT()
struct FDatabaseData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString Host;

	UPROPERTY(EditAnywhere)
	FString User;

	UPROPERTY(EditAnywhere)
	FString Password;

	UPROPERTY(EditAnywhere)
	FString DatabaseName;

	FDatabaseData()
	{
		Host = "";
		User = "";
		Password = "";
		DatabaseName = "";
	}

	FDatabaseData(FString NewHost, FString NewUser, FString NewPassword, FString NewDatabaseName)
	{
		Host = NewHost;
		User = NewUser;
		Password = NewPassword;
		DatabaseName = NewDatabaseName;
	}

	bool IsAllDataSet()
	{
		return !Host.IsEmpty() && !User.IsEmpty() && !Password.IsEmpty() && !DatabaseName.IsEmpty();
	}
};

USTRUCT()
struct FQueryResultRow
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TMap<FString, FString> Fields;
};

USTRUCT()
struct FQueryResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FQueryResultRow> ResultRows;

	UPROPERTY()
	bool Success;

	UPROPERTY()
	FString ErrorMessage;

	FQueryResult()
	{
		Success = false;
		ErrorMessage = "";
	}

	void SetErrorMessage(FString NewErrorMessage)
	{
		ErrorMessage = NewErrorMessage;
		Success = !NewErrorMessage.IsEmpty();
	}
};

struct FResultField
{
	FString StringValue;
	double DoubleValue;
	int64 IntValue;

	FString Name;
	EQueryResultValType Type;

	FString ToString()
	{
		if (Type == EQueryResultValType::Varchar)
			return StringValue;
		if (Type == EQueryResultValType::Int)
			return FString::Printf(TEXT("%i"), IntValue);
		return StringValue;
	}
};

struct FResultValue
{
	TArray<FResultField> Fields;
};

// The internal result object.
struct MSQLConnectorQueryResult
{
	bool Success;
	FString ErrorMessage;
	TArray<FResultValue> Results;

	void SetErrorMessage(FString NewErrorMessage)
	{
		ErrorMessage = NewErrorMessage;
		Success = !NewErrorMessage.IsEmpty();
	}
};

USTRUCT()
struct FCreateTableStructData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString TableName;

	UPROPERTY()
	TMap<FString, ECreatTableFieldType> Fields;

	FCreateTableStructData()
	{
		TableName = "";
	}

	FCreateTableStructData(FString NewTableName)
	{
		TableName = NewTableName;
	}

	void AddField(FString FieldName, ECreatTableFieldType Type)
	{
		Fields.Add(FieldName, Type);
	}
};