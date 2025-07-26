// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerWidget/Chat/MChatWidget.h"
#include "../../../../Public/GameFramework/MPlayerController.h"

TSharedRef<SWidget> UMChatWidget::RebuildWidget()
{
	if (IsValid(GetOwningPlayer()))
	{
		if (IsValid(GetOwningPlayer()->GetHUD()))
		{
			ChatWidget = SNew(MSChatWidget).OwnerHUD(GetOwningPlayer()->GetHUD());
		}
	};

	if (!ChatWidget)
	{
		ChatWidget = SNew(MSChatWidget).OwnerHUD(nullptr);
	}

	return ChatWidget.ToSharedRef();
}

void UMChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);

	if (AMPlayerController* PlayerController = Cast<AMPlayerController>(GetOwningPlayer()))
	{
		PlayerController->UpdateChatWidgetDelegate.BindDynamic(this, &UMChatWidget::OnUpdateChat);
	}
}

FReply UMChatWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	FReply Result = Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	FSlateApplication::Get().SetKeyboardFocus(ChatWidget);

	return Result;
}

void UMChatWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
}

void UMChatWidget::OnUpdateChat()
{
	if (ChatWidget.IsValid())
	{
		ChatWidget->UpdateChatBox();
	}
}
