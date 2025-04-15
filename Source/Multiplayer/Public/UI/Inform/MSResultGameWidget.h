// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"

#include "GameFramework/HUD.h"

/**
 * 
 */
class MULTIPLAYER_API MSResultGameWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(MSResultGameWidget) : _OwnerHUD(), _bResultOfGame(){}

	SLATE_ARGUMENT(TWeakObjectPtr<AHUD>, OwnerHUD)

	SLATE_ARGUMENT(bool, bResultOfGame)

	SLATE_END_ARGS()

private:
	TWeakObjectPtr<AHUD> OwnerHUD; 

public:
	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override;

private:
	FReply OnConfirm() const;
};
