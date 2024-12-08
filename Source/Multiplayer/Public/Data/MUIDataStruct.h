#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SlateBasics.h"

#include "MUIDataStruct.generated.h"

UENUM(BlueprintType)
enum class ETypeOfWidget : uint8
{
	None = 0,
	LoginIn,
	Registration,
	Menu,
	CreateSession,
	FindSession,
	Inform,
	JoinSession
};

USTRUCT()
struct FStyleWidgetData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY()
	FMargin ContentPadding;

	UPROPERTY(EditAnywhere)
	FMargin ButtontPadding;

	UPROPERTY(EditAnywhere)
	FSlateFontInfo ButtonTextStyle;

	UPROPERTY(EditAnywhere)
	FSlateFontInfo TitleTextStyle;

	FStyleWidgetData()
	{
		ContentPadding = FMargin(100.0f);
		ButtontPadding = FMargin(10.0f);

		ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
		ButtonTextStyle.Size = 40.0f;

		TitleTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
		TitleTextStyle.Size = 60.0f;
	}
};

USTRUCT()
struct FInformativeWidgetData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere)
	FText Text;

	UPROPERTY(EditAnywhere)
	bool bWarning;

	UPROPERTY(EditAnywhere)
	bool bWaitingWidget;

	UPROPERTY(EditAnywhere)
	ETypeOfWidget TypeOfNextWidget;

	FInformativeWidgetData()
	{
		Text = FText::GetEmpty();
		bWarning = false;
		bWaitingWidget = false;
		TypeOfNextWidget = ETypeOfWidget::None;
	}

	FInformativeWidgetData(FText NewText)
	{
		Text = NewText;
		bWarning = false;
		bWaitingWidget = false;
		TypeOfNextWidget = ETypeOfWidget::None;
	}

	FInformativeWidgetData(FText NewText, bool NewWarning)
	{
		Text = NewText;
		bWarning = NewWarning;
		bWaitingWidget = false;
		TypeOfNextWidget = ETypeOfWidget::None;
	}

	FInformativeWidgetData(FText NewText, ETypeOfWidget NewTypeOfNextWidget)
	{
		Text = NewText;
		bWarning = true;
		bWaitingWidget = false;
		TypeOfNextWidget = NewTypeOfNextWidget;
	}

	FInformativeWidgetData(FText NewText, bool NewWarning, ETypeOfWidget NewTypeOfNextWidget)
	{
		Text = NewText;
		bWarning = NewWarning;
		bWaitingWidget = false;
		TypeOfNextWidget = NewTypeOfNextWidget;
	}

	FInformativeWidgetData(FText NewText, bool NewWarning, bool NewWaitingWidget, ETypeOfWidget NewTypeOfNextWidget)
	{
		Text = NewText;
		bWarning = NewWarning;
		bWaitingWidget = NewWaitingWidget;
		TypeOfNextWidget = NewTypeOfNextWidget;
	}
};
