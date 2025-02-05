// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MPlayerInfoSubsystem.h"

void UMPlayerInfoSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SetDataBase();
}

void UMPlayerInfoSubsystem::Deinitialize()
{
	if (!ChatTableName.IsEmpty())
		DestroyTable(ChatTableName);

	CloseDataBase();

	Super::Deinitialize();
}

bool UMPlayerInfoSubsystem::IsUserSignIn()
{
	return !LoginOfUser.IsEmpty();
}

int UMPlayerInfoSubsystem::GetCharacterType()
{
	if (!IsValid(Database))
	{
		delete Database;
		SetDataBase();
	}

	if (IsValid(Database) && !LoginOfUser.IsEmpty())
	{
		FQueryResult Result =
			Database->GetQueryData("SELECT * FROM Users Where Login = " + LoginOfUser + " ;");
		if (Result.ResultRows.Num() > 0 && Result.Success)
		{
			return FCString::Atoi(**Result.ResultRows[0].Fields.Find("CharacterType"));
		}
	}

	return !LoginOfUser.IsEmpty() ? 0 : -1;
}

bool UMPlayerInfoSubsystem::IsUserDataCorrect(FString Login, FString Password)
{
	if (IsValid(Database))
	{
		if (Database->GetQueryData("SELECT * FROM Users Where Login = " + Login + " AND Password = " + Password + " ;").Success)
		{
			LoginOfUser = Login;
			return true;
		}
	}

	return false;
}

bool UMPlayerInfoSubsystem::AddMessage(FString Message)
{
	if (IsValid(Database) && !Message.IsEmpty())
	{
		if (Database->ExecuteQuery("INSERT INTO " + ChatTableName + " (User, Message) VALUES('"
			+ LoginOfUser + "' , '" +
			Message + "' );"))
		{
			return true;
		}
	}

	return false;
}

bool UMPlayerInfoSubsystem::CreateChatTable(FCreateTableStructData* TableData)
{
	if (IsValid(Database))
	{
		ChatTableName = TableData->TableName;

		TableData->AddField("DateTime", ECreatTableFieldType::Datetime);
		TableData->AddField("User", ECreatTableFieldType::Varchar);
		TableData->AddField("Message", ECreatTableFieldType::Text);

		return CreateTable(TableData);
	}
	
	return false;
}

bool UMPlayerInfoSubsystem::DestroyTable(FString TableName)
{
	if (IsValid(Database))
	{
		return Database->GetQueryData("DROP TABLE " + TableName + " ;").Success;
	}

	return false;
}

void UMPlayerInfoSubsystem::SetChatTableName(FString Value)
{
	ChatTableName = Value;
}

FChatMessageData* UMPlayerInfoSubsystem::GetLastMessageData()
{
	if (IsValid(Database))
	{
		FQueryResult QueryResult = Database->GetQueryData("SELECT* FROM " + ChatTableName + " WHERE id = (SELECT max(id) FROM " + ChatTableName + ");");
		if (QueryResult.ResultRows.Num() == 1 && QueryResult.Success)
		{
			return new FChatMessageData(QueryResult.ResultRows[0].Fields);
		}
	}

	return nullptr;
}

bool UMPlayerInfoSubsystem::IsPlayerRegistered(FString Login)
{
	if (IsValid(Database))
	{
		return Database->GetQueryData("SELECT * FROM Users Where Login = " + Login + " ;").Success;
	}

	return false;
}

bool UMPlayerInfoSubsystem::RegisterPlayerData(FPlayerInfoData* NewPlayerData)
{
	if (IsValid(Database))
	{
		if (Database->ExecuteQuery("INSERT INTO Users (Login, Password, CharacterType) VALUES("
			+ NewPlayerData->Login + " , " + 
			NewPlayerData->Password + " , " +
			FString::FromInt(NewPlayerData->TypeOfCharacter) + " );"))
		{
			return true;
		}
	}

	return false;
}

void UMPlayerInfoSubsystem::SetDataBase()
{
	Database = NewObject<UMSQLDatabase>();

	if (IsValid(Database))
	{
		if (!Database->InitConnection())
		{
			delete Database;
		}
	}
}

void UMPlayerInfoSubsystem::CloseDataBase()
{
	if (IsValid(Database))
	{
		Database->IsCloseConnection();
	}
}

bool UMPlayerInfoSubsystem::CreateTable(FCreateTableStructData* TableData)
{
	if (IsValid(Database))
	{
		if (TableData->Fields.Num() > 0)
		{
			FString Query = "CREATE TABLE IF NOT EXISTS " + TableData->TableName + " (`Id` INT NOT NULL AUTO_INCREMENT , ";

			for (auto Field : TableData->Fields)
			{
				Query += "`" + Field.Key + "` " + GetTypeForQuery(Field.Value) + " , ";
			}

			Query += "PRIMARY KEY(`Id`));";

			return Database->ExecuteQuery(Query);
		}
	}

	return false;
}

FString UMPlayerInfoSubsystem::GetTypeForQuery(ECreatTableFieldType Type)
{
	FString Result = "";

	switch (Type)
	{
	case ECreatTableFieldType::Int:
		Result += "INT NOT NULL";
		break;
	case ECreatTableFieldType::Varchar:
		Result += "VARCHAR(255) NOT NULL";
		break;
	case ECreatTableFieldType::Text:
		Result += "TEXT NOT NULL";
		break;
	case ECreatTableFieldType::Datetime:
		Result += "DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP";
		break;
	default:
		break;
	}

	return Result;
}
