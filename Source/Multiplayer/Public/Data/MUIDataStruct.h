#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "MUIDataStruct.generated.h"

UENUM(BlueprintType)
enum class ETypeOfWidget : uint8
{
	None = 0,
	LoginIn,
	Registration,
	Menu,
	CreateSession,
	FindSession
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
	ETypeOfWidget TypeOfNextWidget;

	FInformativeWidgetData()
	{
		Text = FText::GetEmpty();
		bWarning = false;
		TypeOfNextWidget = ETypeOfWidget::None;
	}

	FInformativeWidgetData(FText NewText)
	{
		Text = NewText;
		bWarning = false;
		TypeOfNextWidget = ETypeOfWidget::None;
	}

	FInformativeWidgetData(FText NewText, bool NewWarning)
	{
		Text = NewText;
		bWarning = NewWarning;
		TypeOfNextWidget = ETypeOfWidget::None;
	}

	FInformativeWidgetData(FText NewText, ETypeOfWidget NewTypeOfNextWidget)
	{
		Text = NewText;
		bWarning = true;
		TypeOfNextWidget = NewTypeOfNextWidget;
	}

	FInformativeWidgetData(FText NewText, bool NewWarning, ETypeOfWidget NewTypeOfNextWidget)
	{
		Text = NewText;
		bWarning = NewWarning;
		TypeOfNextWidget = NewTypeOfNextWidget;
	}
};