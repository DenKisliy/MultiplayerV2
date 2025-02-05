// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerWidget/Chat/MChatWidget.h"
#include "../../../../Public/GameFramework/MPlayerController.h"

TSharedRef<SWidget> UMChatWidget::RebuildWidget()
{
	if (!ChatWidget)
	{
		ChatWidget = SNew(MSChatWidget).OwnerHUD(nullptr);
	}

	if (IsValid(GetOwningPlayer()))
	{
		if (IsValid(GetOwningPlayer()->GetHUD()))
		{
			ChatWidget = SNew(MSChatWidget).OwnerHUD(GetOwningPlayer()->GetHUD());
		}
	}

	return ChatWidget.ToSharedRef();
}

void UMChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsFocusable = true;

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

void UMChatWidget::OnUpdateChat()
{
	if (ChatWidget.IsValid())
	{
		ChatWidget->UpdateChatBox();
	}
}
