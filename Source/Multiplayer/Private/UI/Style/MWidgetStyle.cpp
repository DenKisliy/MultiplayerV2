// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Style/MWidgetStyle.h"
#include "CoreUObject.h"

UMWidgetStyle::UMWidgetStyle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FMargin UMWidgetStyle::GetContentPadding()
{
	return FMargin(100.0f);
}

FMargin UMWidgetStyle::GetButtontPadding()
{
	return FMargin(10.0f);
}

FSlateFontInfo UMWidgetStyle::GetButtonTextStyle()
{
	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 40.0f;
	return ButtonTextStyle;
}

FSlateFontInfo UMWidgetStyle::GetTitleTextStyle()
{
	FSlateFontInfo TitleTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	TitleTextStyle.Size = 60.0f;
	return TitleTextStyle;
}

FSlateFontInfo UMWidgetStyle::GetChatTextStyle()
{
	FSlateFontInfo ChatTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ChatTextStyle.Size = 17.0f;
	return ChatTextStyle;
}