// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MPlayerInfoSubsystem.h"

void UMPlayerInfoSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SetDataBase();
}

void UMPlayerInfoSubsystem::Deinitialize()
{
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
