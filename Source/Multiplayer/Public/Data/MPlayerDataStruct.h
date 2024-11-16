#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "MPlayerDataStruct.generated.h"

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

	FPlayerInfoData()
	{
		Login = "";
		Password = "";
		TypeOfCharacter = 0;
	}

	FPlayerInfoData(FString NewLogin, FString NewPassword, int NewTypeOfCharacter)
	{
		Login = NewLogin;
		Password = NewPassword;
		TypeOfCharacter = NewTypeOfCharacter;
	}
};