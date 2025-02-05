// Fill out your copyright notice in the Description page of Project Settings.


#include "SQL/MSQLDatabase.h"

UMSQLDatabase::UMSQLDatabase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UMSQLDatabase::InitConnection()
{
	if (FDatabaseData* DatabaseData = GetDefaultSettings())
	{
		string HostString(TCHAR_TO_UTF8(*DatabaseData->Host));
		string UserNameString(TCHAR_TO_UTF8(*DatabaseData->User));
		string UserPasswordString(TCHAR_TO_UTF8(*DatabaseData->Password));
		string DatabaseNameString(TCHAR_TO_UTF8(*DatabaseData->DatabaseName));

		DatabaseConnection = NewObject<UMSQLConnection>();

		if (mysql_library_init(0, nullptr, nullptr) != 0)
		{
			mysql_library_end();
			DatabaseConnection = nullptr;
			return false;
		}

		DatabaseConnection->GlobalConnection = mysql_init(nullptr);

		if (!DatabaseConnection->GlobalConnection)
		{
			mysql_library_end();
			DatabaseConnection = nullptr;
			return false;
		}

		if (!mysql_real_connect(DatabaseConnection->GlobalConnection,
			HostString.c_str(),
			UserNameString.c_str(),
			UserPasswordString.c_str(),
			DatabaseNameString.c_str(),
			3306, nullptr, 0))
		{
			FString error = mysql_error(DatabaseConnection->GlobalConnection);

			UMSQLConnection::CloseConnection(DatabaseConnection);
			DatabaseConnection = nullptr;
			return false;
		}

		if (mysql_set_character_set(DatabaseConnection->GlobalConnection, "utf8") != 0)
		{
			UMSQLConnection::CloseConnection(DatabaseConnection);
			DatabaseConnection = nullptr;
			return false;
		}

		return true;
	}

	return false;
}

bool UMSQLDatabase::IsCloseConnection()
{
	bool Result = UMSQLConnection::CloseConnection(DatabaseConnection);
	if (Result)
	{
		DatabaseConnection = nullptr;
	}
	return Result;
}

bool UMSQLDatabase::ExecuteQuery(FString Query)
{
	if (IsValid(DatabaseConnection))
	{
		if (!DatabaseConnection->GlobalConnection || !DatabaseConnection->CheckConnection())
		{
			return false;
		};

		string QueryString(TCHAR_TO_UTF8(*Query));

		if (mysql_query(DatabaseConnection->GlobalConnection, QueryString.c_str()))
		{
			return false;
		}

		return true;
	}

	return false;
}

#pragma warning(push)
#pragma warning( disable : 4706 )
MSQLConnectorQueryResult* UMSQLDatabase::RunQueryAndGetResults(FString Query)
{
	MSQLConnectorQueryResult* resultOutput = new MSQLConnectorQueryResult();
	if (!DatabaseConnection)
	{
		resultOutput->SetErrorMessage("Connection is NULL!");
		return resultOutput;
	};

	if (!DatabaseConnection->CheckConnection())
	{
		resultOutput->SetErrorMessage("Not connected!");
		return resultOutput;
	};
	
	string MyStdString(TCHAR_TO_UTF8(*Query));

	mysql_query(DatabaseConnection->GlobalConnection, MyStdString.c_str());
	
	MYSQL_RES* result = mysql_store_result(DatabaseConnection->GlobalConnection);

	if (!result)
	{
		resultOutput->SetErrorMessage("Result is NULL!");
		return resultOutput;
	}

	int num_fields = mysql_num_fields(result);

	TArray<int> fieldTypes;
	TArray<FString> fieldNames;

	if (MYSQL_FIELD * fields = mysql_fetch_fields(result))
	{
		for (int i = 0; i < num_fields; i++)
		{
			FString NewString = FString::FromInt(fields[i].type);

			fieldTypes.Add(fields[i].type);
			fieldNames.Add(UTF8_TO_TCHAR(fields[i].name));
		}
	}

	TArray<FResultValue> resultRows;

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		FResultValue rowVal;

		for (int i = 0; i < num_fields; i++)
		{
			FResultField val;

			FString columnNameStr = fieldNames[i];
			val.Name = columnNameStr;

			FString fieldValueStr = (UTF8_TO_TCHAR(row[i]));

			switch (fieldTypes[i])
			{
			case enum_field_types::MYSQL_TYPE_LONG:
				val.Type = EQueryResultValType::Int;
				val.IntValue = FCString::Atoi(*fieldValueStr);
				break;
			case enum_field_types::MYSQL_TYPE_VAR_STRING:
				val.Type = EQueryResultValType::Varchar;
				val.StringValue = fieldValueStr;
				break;
			default:
				val.Type = EQueryResultValType::UnsupportedValueType;
				val.IntValue = FCString::Atoi(*fieldValueStr);
				val.StringValue = fieldValueStr;
			}

			rowVal.Fields.Add(val);
		}

		resultRows.Add(rowVal);
	}

	mysql_free_result(result);

	resultOutput->Results = resultRows;
	resultOutput->Success = true;
	return resultOutput;
}

FQueryResult UMSQLDatabase::GetQueryData(const FString& Query)
{
	FQueryResult Result;

	if (IsValid(DatabaseConnection))
	{
		if (!DatabaseConnection)
		{
			Result.SetErrorMessage("Not connected!");
			return Result;
		}
		
		if (!DatabaseConnection->CheckConnection())
		{
			Result.SetErrorMessage("Connection is NULL!");
			return Result;
		}

		MSQLConnectorQueryResult* QueryResult = RunQueryAndGetResults(Query);
		Result.SetErrorMessage(QueryResult->ErrorMessage);
		Result.Success = QueryResult->Results.Num() > 0;

		for (FResultValue Row : QueryResult->Results)
		{
			FQueryResultRow OutRow;
			for (auto Field : Row.Fields)
			{
				OutRow.Fields.Add(Field.Name, Field.ToString());
			}
			Result.ResultRows.Add(OutRow);
		}
	}
	return Result;
}

FDatabaseData* UMSQLDatabase::GetDefaultSettings()
{
	FDatabaseData* DatabaseData = new FDatabaseData();

	GConfig->GetString(
		TEXT("/Script/DatabaseSettings"),
		TEXT("Host"),
		DatabaseData->Host,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/DatabaseSettings"),
		TEXT("UserName"),
		DatabaseData->User,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/DatabaseSettings"),
		TEXT("UserPassword"),
		DatabaseData->Password,
		GGameIni
	);

	GConfig->GetString(
		TEXT("/Script/DatabaseSettings"),
		TEXT("DatabaseName"),
		DatabaseData->DatabaseName,
		GGameIni
	);

	return DatabaseData->IsAllDataSet() ? DatabaseData : nullptr;
}
