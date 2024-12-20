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
	SLATE_BEGIN_ARGS(MSInformativeWidget) : _OwnerHUD(), _WidgetData() {}

	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnerHUD)

	SLATE_ATTRIBUTE(TSharedPtr<FInformativeWidgetData>, WidgetData);

	SLATE_END_ARGS()

private:
	TWeakObjectPtr<AHUD> OwnerHUD;

	TAttribute<TSharedPtr<FInformativeWidgetData>> WidgetData;

	TSharedPtr<SVerticalBox> WidgetBox;

public:
	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override;

private:
	FReply OnConfirm() const;
};
