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
	int CountOfWin;

	UPROPERTY(EditAnywhere)
	int CountOfLost;

	UPROPERTY(EditAnywhere)
	int ResultOfLastGame;

	FPlayerResultGameData(FString NewUserName = "", int NewCountOfWin = 0, int NewCountOfLost = 0, int NewResultOfLastGame = -1)
	{
		UserName = NewUserName;
		CountOfWin = NewCountOfWin;
		CountOfLost = NewCountOfLost;
		ResultOfLastGame = NewResultOfLastGame;
	}

	FPlayerResultGameData(TMap<FString, FString> Field)
	{
		UserName = **Field.Find("User");
		CountOfWin = FCString::Atoi(**Field.Find("CountOfWin"));
		CountOfLost = FCString::Atoi(**Field.Find("CountOfLost"));
		ResultOfLastGame = FCString::Atoi(**Field.Find("ResultOfLastGame"));
	}
};