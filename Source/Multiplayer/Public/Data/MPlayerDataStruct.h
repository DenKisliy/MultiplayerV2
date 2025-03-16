#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

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