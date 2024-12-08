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
class MULTIPLAYER_API MSFindSessionWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(MSFindSessionWidget) : _OwnerHUD(), _FindSessions() {}

	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnerHUD)

	SLATE_ATTRIBUTE(TArray<FOnlineSessionSearchResult>, FindSessions)

	SLATE_END_ARGS()

private:
	TWeakObjectPtr<AHUD> OwnerHUD;

	TSharedPtr<SVerticalBox> SessionsButtonsBox;

public:
	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override;

private:
	void AddButtonsForSessions(TArray<FOnlineSessionSearchResult> FindSessionsData);

	FReply OnCloseWindow() const;

	FString GetSessionName(FOnlineSessionSearchResult& SessionData);
};
