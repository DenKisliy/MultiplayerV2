#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SlateBasics.h"
#include "../UI/Style/MWidgetStyle.h"

#include "MUIDataStruct.generated.h"

UENUM(BlueprintType)
enum class ETypeOfWidget : uint8
{
	None = 0,
	LoginIn,
	Registration,
	GameTypeMenu,
	MultiplayerMenu,
	CreateSession,
	FindSession,
	Inform,
	JoinSession,
	ResultOfGame
};

USTRUCT()
struct FChatMessageData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere)
	FString DateTime;

	UPROPERTY(EditAnywhere)
	FString User;

	UPROPERTY(EditAnywhere)
	FString Message;

	FChatMessageData(FString DateTimeValue = "", FString UserValue = "", FString MessageValue = "")
	{
		DateTime = DateTimeValue;
		User = UserValue;
		Message = MessageValue;
	}

	FChatMessageData(TMap<FString, FString> Data)
	{
		DateTime = *Data.Find("DateTime");
		User = *Data.Find("User");
		Message = *Data.Find("Message");
	}
};

USTRUCT()
struct FUserRatingData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere)
	FString User;

	UPROPERTY(EditAnywhere)
	int CountOfWin;

	UPROPERTY(EditAnywhere)
	int CountOfLost;

	UPROPERTY(EditAnywhere)
	int ResultOfLastGame;

	FUserRatingData(FString UserValue = "", int CountOfWinValue = 0, int CountOfLostValue = 0, int ResultOfLastGameValue = 0)
	{
		User = UserValue;
		CountOfWin = CountOfWinValue;
		CountOfLost = CountOfLostValue;
		ResultOfLastGame = ResultOfLastGameValue;
	}

	FUserRatingData(TMap<FString, FString> Data)
	{
		User = *Data.Find("User");
		CountOfWin = FCString::Atoi(**Data.Find("CountOfWin"));
		CountOfLost = FCString::Atoi(**Data.Find("CountOfLost"));
		ResultOfLastGame = FCString::Atoi(**Data.Find("ResultOfLastGame"));
	}
};

USTRUCT()
struct FInformWidgetData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere)
	FText Text;
	
	UPROPERTY(EditAnywhere)
	bool bWarning;

	UPROPERTY(EditAnywhere)
	bool bWaiting;

	UPROPERTY(EditAnywhere)
	ETypeOfWidget NextWidgetType;

	UPROPERTY(EditAnywhere)
	ETypeOfWidget PreviousWidgetType;

	FInformWidgetData(FText TextValue = FText::GetEmpty(), bool bWarningValue = false, bool bWaitingValue = false,
		ETypeOfWidget NextWidgetTypeValue = ETypeOfWidget::None, ETypeOfWidget PreviousWidgetTypeValue = ETypeOfWidget::None)
	{
		Text = TextValue;
		bWarning = bWarningValue;
		bWaiting = bWaitingValue;
		NextWidgetType = NextWidgetTypeValue;
		PreviousWidgetType = PreviousWidgetTypeValue;
	}
};
