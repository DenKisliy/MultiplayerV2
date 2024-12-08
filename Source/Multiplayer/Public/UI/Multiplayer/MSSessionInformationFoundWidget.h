// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"

#include "GameFramework/HUD.h"
#include "../../Data/MUIDataStruct.h"

#include "../../Subsystem/MPlayerInfoSubsystem.h"

class MULTIPLAYER_API MSSessionInformationFoundWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(MSSessionInformationFoundWidget) : _OwnerHUD(), _SessionName() {}

	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnerHUD)

	SLATE_ATTRIBUTE(FString, SessionName)

	SLATE_END_ARGS()

private:
	TWeakObjectPtr<AHUD> OwnerHUD;

	FString SessionName;

public:
	void Construct(const FArguments& InArgs);

private:
	FReply OnJoinSession() const;

	void ShowInformWidget(FInformativeWidgetData* InformWidgetData) const;
};
