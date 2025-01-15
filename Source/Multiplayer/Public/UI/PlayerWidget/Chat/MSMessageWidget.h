// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"

#include "GameFramework/HUD.h"
#include "../../../Data/MUIDataStruct.h"

class MULTIPLAYER_API MSMessageWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(MSMessageWidget) : _MessageData() {}

	SLATE_ATTRIBUTE(TSharedPtr<FChatMessageData>, MessageData)

	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);
};
