#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MSQLDataStruct.h"

#include "MPlayerDataStruct.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerDeath, bool, bDeathPlayer);


UENUM(BlueprintType)
enum class ETypeOfCharacter : uint8
{
	None = 0,
	Greystone,
	Fey,
};

USTRUCT()
struct FPlayerInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere)
	FString Login;

	UPROPERTY(EditAnywhere)
	FString Password;

	UPROPERTY(EditAnywhere)
	int TypeOfCharacter;

	FPlayerInfoData(FString NewLogin = "", FString NewPassword = "", int NewTypeOfCharacter = 0)
	{
		Login = NewLogin;
		Password = NewPassword;
		TypeOfCharacter = NewTypeOfCharacter;
	}
};

USTRUCT()
struct FPlayerResultGameData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere)
	FString UserName;

	UPROPERTY(EditAnywhere)
	int CountOfMultiplayerWin;

	UPROPERTY(EditAnywhere)
	int CountOfMultiplayerLost;

	UPROPERTY(EditAnywhere)
	int CountOfStandaloneWin;

	UPROPERTY(EditAnywhere)
	int CountOfStandaloneLost;

	UPROPERTY(EditAnywhere)
	int ResultOfLastGame;

	FPlayerResultGameData(FString NewUserName = "")
	{
		UserName = NewUserName; 
		CountOfMultiplayerWin = 0;
		CountOfMultiplayerLost = 0;
		CountOfStandaloneWin = 0;
		CountOfStandaloneLost = 0;
		ResultOfLastGame = -1;
	}

	FPlayerResultGameData(TMap<FString, FString> Field)
	{
		UserName = **Field.Find("User");
		CountOfMultiplayerWin = FCString::Atoi(**Field.Find("CountOfMultiplayerWin"));
		CountOfMultiplayerLost = FCString::Atoi(**Field.Find("CountOfMultiplayerLost"));
		CountOfStandaloneWin = FCString::Atoi(**Field.Find("CountOfStandaloneWin"));
		CountOfStandaloneLost = FCString::Atoi(**Field.Find("CountOfStandaloneLost"));
		ResultOfLastGame = FCString::Atoi(**Field.Find("ResultOfLastGame"));
	}

	void UpdateDataOfGame(int32 ResultOfGame, bool bStandalone)
	{
		ResultOfLastGame = ResultOfGame;
		if (ResultOfGame)
		{
			if (bStandalone)
			{
				CountOfStandaloneWin = CountOfStandaloneWin + 1;
			}
			else
			{
				CountOfMultiplayerWin = CountOfMultiplayerWin + 1;
			}
		}
		else
		{
			if (bStandalone)
			{
				CountOfStandaloneLost = CountOfStandaloneLost + 1;
			}
			else
			{
				CountOfMultiplayerLost = CountOfMultiplayerLost + 1;
			}
		}
	}
};