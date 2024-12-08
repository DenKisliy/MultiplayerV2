// Fill out your copyright notice in the Description page of Project Settings.


#include "SQL/MSQLConnection.h"
#include "CoreUObject.h"

UMSQLConnection::UMSQLConnection(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UMSQLConnection::CheckConnection()
{
	if (GlobalConnection)
	{
		return mysql_ping(GlobalConnection) == 0;
	}
	return false;
}

bool UMSQLConnection::CloseConnection(UMSQLConnection* Connection)
{
	if (Connection) 
	{
		if (Connection->CheckConnection())
		{
			mysql_close(Connection->GlobalConnection);
			Connection->GlobalConnection = nullptr;
			mysql_library_end();
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MySQLCloseConnection: Connection is valid but Server does no respond!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MySQLCloseConnection: Connection is null!"));
	}
	return false;
}
