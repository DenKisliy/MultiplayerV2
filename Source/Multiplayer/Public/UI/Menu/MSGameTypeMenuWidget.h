// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "../../Data/MUIDataStruct.h"

#include "GameFramework/HUD.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class MULTIPLAYER_API MSGameTypeMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(MSGameTypeMenuWidget) : _OwnerHUD() {}

	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnerHUD)

	SLATE_END_ARGS()

private:
	TWeakObjectPtr<AHUD> OwnerHUD;

public:
	void Construct(const FArguments& InArgs);	
	
	virtual bool SupportsKeyboardFocus() const override;

private:
	FReply OnLaunchStandAloneMode() const;

	FReply OnGoToNextMultiplayerMenu() const;

	FReply OnBackToPreviousMenu() const;

	void OpenNextWidget(ETypeOfWidget Type) const;
};
