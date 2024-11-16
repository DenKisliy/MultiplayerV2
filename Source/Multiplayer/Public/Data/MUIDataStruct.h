#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "MUIDataStruct.generated.h"

UENUM(BlueprintType)
enum class ETypeOfUIWidget : uint8
{
	None = 0,
	LoginIn,
	Registration,
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
	ETypeOfUIWidget TypeOfNextWidget;

	FInformativeWidgetData()
	{
		Text = FText::GetEmpty();
		bWarning = false;
		TypeOfNextWidget = ETypeOfUIWidget::None;
	}

	FInformativeWidgetData(FText NewText)
	{
		Text = NewText;
		bWarning = false;
		TypeOfNextWidget = ETypeOfUIWidget::None;
	}

	FInformativeWidgetData(FText NewText, bool NewWarning)
	{
		Text = NewText;
		bWarning = NewWarning;
		TypeOfNextWidget = ETypeOfUIWidget::None;
	}

	FInformativeWidgetData(FText NewText, ETypeOfUIWidget NewTypeOfNextWidget)
	{
		Text = NewText;
		bWarning = true;
		TypeOfNextWidget = NewTypeOfNextWidget;
	}

	FInformativeWidgetData(FText NewText, bool NewWarning, ETypeOfUIWidget NewTypeOfNextWidget)
	{
		Text = NewText;
		bWarning = NewWarning;
		TypeOfNextWidget = NewTypeOfNextWidget;
	}
};