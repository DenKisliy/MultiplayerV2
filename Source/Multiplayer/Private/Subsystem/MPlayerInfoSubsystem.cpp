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

bool UMPlayerInfoSubsystem::IsPlayerRegistered(FString Login)
{
	const TCHAR* SQLQuery = TEXT("select * from users where Login = $Login");
	FSQLitePreparedStatement LoadStatement;
	LoadStatement.Create(*MultiplayerDb, SQLQuery, ESQLitePreparedStatementFlags::Persistent);

	if (MultiplayerDb->IsValid() && LoadStatement.IsValid())
	{
		LoadStatement.Reset();

		if (LoadStatement.SetBindingValueByName(TEXT("$Login"), Login) && LoadStatement.Execute())
		{
			return LoadStatement.Step() == ESQLitePreparedStatementStepResult::Row;
		}
	}

	return false;
}

bool UMPlayerInfoSubsystem::IsLoginPasswordCorrect(FString Login, FString Password)
{
	if (FPlayerInfoData* FindPlayerData = GetPlayerInfo(Login))
	{
		if (FindPlayerData->Password == Password)
		{
			return true;
		}
	}

	return false;
}

bool UMPlayerInfoSubsystem::RegisterPlayerData(FPlayerInfoData* NewPlayerData)
{
	const TCHAR* SQLQuery = TEXT("INSERT INTO users (Login, Password, TypeOfCharacter) VALUES($Login, $Password, $TypeOfCharacter)");
	FSQLitePreparedStatement PreparedStatement;
	PreparedStatement.Create(*MultiplayerDb, SQLQuery, ESQLitePreparedStatementFlags::Persistent);

	if (MultiplayerDb->IsValid() && PreparedStatement.IsValid())
	{
		PreparedStatement.Reset();

		bool bBindingSuccess = true;
		bBindingSuccess = bBindingSuccess && PreparedStatement.SetBindingValueByName(TEXT("$Login"), NewPlayerData->Login);
		bBindingSuccess = bBindingSuccess && PreparedStatement.SetBindingValueByName(TEXT("$Password"), NewPlayerData->Password);
		bBindingSuccess = bBindingSuccess && PreparedStatement.SetBindingValueByName(TEXT("$TypeOfCharacter"), NewPlayerData->TypeOfCharacter);

		if (!bBindingSuccess)
		{
			return false;
		}

		return PreparedStatement.Execute();
	}

	return false;
}

FPlayerInfoData* UMPlayerInfoSubsystem::GetPlayerInfo(FString Login)
{
	const TCHAR* SQLQuery = TEXT("select * from users where Login = $Login");
	FSQLitePreparedStatement PreparedStatement;
	PreparedStatement.Create(*MultiplayerDb, SQLQuery, ESQLitePreparedStatementFlags::Persistent);

	if (MultiplayerDb->IsValid() && PreparedStatement.IsValid())
	{
		PreparedStatement.Reset();

		PreparedStatement.SetBindingValueByName(TEXT("$Login"), Login);

		if (PreparedStatement.Execute() && PreparedStatement.Step() == ESQLitePreparedStatementStepResult::Row)
		{
			FPlayerInfoData* PlayerInfo = new FPlayerInfoData();
			PlayerInfo->Login = Login;
			PreparedStatement.GetColumnValueByName(TEXT("Password"), PlayerInfo->Password);
			PreparedStatement.GetColumnValueByName(TEXT("TypeOfCharacter"), PlayerInfo->TypeOfCharacter);

			return PlayerInfo;
		}
	}

	return nullptr;
}

void UMPlayerInfoSubsystem::SetDataBase()
{
	FString dbPath = FPaths::ProjectContentDir() + "DataBase/Multiplayer.db";
	MultiplayerDb = new FSQLiteDatabase();
	
	if (!MultiplayerDb->Open(*dbPath, ESQLiteDatabaseOpenMode::ReadWrite))
	{
		UE_LOG(LogTemp, Fatal, TEXT("%s"), TEXT("Couldn't open db."));
	}
}

void UMPlayerInfoSubsystem::CloseDataBase()
{
	if (!MultiplayerDb->Close())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to close database: %s"), *MultiplayerDb->GetLastError());
	}
	else
	{
		delete MultiplayerDb;
	}
}
