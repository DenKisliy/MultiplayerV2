// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"

#include "../../Data/MUIDataStruct.h"

/**
 * 
 */
class MULTIPLAYER_API MSInformativeWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(MSInformativeWidget) : _OwnerHUD(), _Text(FText::FromString("")), _Warning(false),
		_Waiting(false), _NextWidget(ETypeOfWidget::None), _PreviousWidget(ETypeOfWidget::None) {}

	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnerHUD)

	SLATE_ATTRIBUTE(FText, Text);

	SLATE_ATTRIBUTE(bool, Warning);

	SLATE_ATTRIBUTE(bool, Waiting);

	SLATE_ATTRIBUTE(ETypeOfWidget, NextWidget);

	SLATE_ATTRIBUTE(ETypeOfWidget, PreviousWidget);

	SLATE_END_ARGS()

private:
	TWeakObjectPtr<AHUD> OwnerHUD;

	TSharedPtr<SVerticalBox> WidgetBox;

	bool bWarning;

	bool bWaiting;

	ETypeOfWidget TypeOfNextWidget;

	ETypeOfWidget TypeOfPreviousWidget;

public:
	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override;

private:
	FReply OnConfirm() const;
};
