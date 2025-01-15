// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"

#include "GameFramework/HUD.h"
#include "../../Data/MUIDataStruct.h"
#include "../../Subsystem/MSessionSubsystem.h"

/**
 * 
 */
class MULTIPLAYER_API MSCreateSessionWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(MSCreateSessionWidget) : _OwnerHUD() {}

	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnerHUD)

	SLATE_END_ARGS()

private:
	TWeakObjectPtr<AHUD> OwnerHUD;

public:
	TSharedPtr<SEditableTextBox> SessionNameBoxPtr;

public:
	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override;

private:
	FReply OnCreateSession() const;

	FReply OnBackToPreviousMenu() const;

	void SetDefault() const;

	void ShowInformWidget(FText Text, bool bWarning = false, bool bWaiting = true, ETypeOfWidget PreviousWidget = ETypeOfWidget::None) const;

	void CreateSession() const;
};
