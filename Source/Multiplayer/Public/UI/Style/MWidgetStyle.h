// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MWidgetStyle.generated.h"
/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UMWidgetStyle : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	static FMargin GetContentPadding();

	static FMargin GetButtontPadding();

	static FSlateFontInfo GetButtonTextStyle();

	static FSlateFontInfo GetTitleTextStyle();

	static FSlateFontInfo GetChatTextStyle();

};
