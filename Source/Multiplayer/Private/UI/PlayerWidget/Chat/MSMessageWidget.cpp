// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerWidget/Chat/MSMessageWidget.h"
#include <Components/SlateWrapperTypes.h>

void MSMessageWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	FChatMessageData* MessageData = InArgs._MessageData.Get().Get();

	ChildSlot
	[
		SNew(SWrapBox).PreferredSize(500.f)
			+ SWrapBox::Slot()
			[
				SNew(STextBlock)
					.Text(FText::FromString(MessageData->DateTime))
					.ColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f))
					.Font(UMWidgetStyle::GetChatTextStyle())
			]
			+ SWrapBox::Slot()
			[
				SNew(STextBlock)
					.Text(FText::FromString(" : "))
					.ColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f))
					.Font(UMWidgetStyle::GetChatTextStyle())
			]
			+ SWrapBox::Slot()
			[
				SNew(STextBlock)
					.Text(FText::FromString(MessageData->User))
					.ColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f))
					.Font(UMWidgetStyle::GetChatTextStyle())
			]
			+ SWrapBox::Slot()
			[
				SNew(STextBlock)
					.Text(FText::FromString(" : "))
					.ColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f))
					.Font(UMWidgetStyle::GetChatTextStyle())
			]
			+ SWrapBox::Slot()
			[
				SNew(STextBlock)
					.Text(FText::FromString(MessageData->Message))
					.ColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.f))
					.Font(UMWidgetStyle::GetChatTextStyle())
			]
	];
}
