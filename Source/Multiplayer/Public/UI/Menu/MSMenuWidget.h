// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"

#include "GameFramework/HUD.h"
#include "../../Data/MUIDataStruct.h"

/**
 * 
 */
class MULTIPLAYER_API MSMenuWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(MSMenuWidget) : _OwnerHUD() {}

	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnerHUD)

	SLATE_END_ARGS()

private:
	TWeakObjectPtr<AHUD> OwnerHUD; 

public:
	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override;

private:
	FReply OnCreateSession() const;

	FReply OnFindSession() const;

	FReply OnBackToPreviousMenu() const;

	void OpenNextWidget(ETypeOfWidget Type) const;

	void ShowInformWidget(FInformativeWidgetData* InformWidgetData) const;
};
