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
	JoinSession
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
