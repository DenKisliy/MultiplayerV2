// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerWidget/Chat/MSChatWidget.h"
#include "../../../../Public/Subsystem/MPlayerInfoSubsystem.h"
#include "../../../../Public/GameFramework/MPlayerController.h"

#define LOCTEXT_NAMESPACE "Chat"

void MSChatWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;

	ChildSlot 
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Left)
		.Padding(15)
		[
			SNew(SBorder).BorderImage(FAppStyle::Get().GetBrush("Brushes.Panel"))
			[

				SNew(SVerticalBox) 
					+ SVerticalBox::Slot().AutoHeight().VAlign(VAlign_Bottom)
					[
						SNew(SBox).WidthOverride(500.0f).HeightOverride(400.0f)
						[
							SAssignNew(ListViewWidget, SListView<TSharedPtr<FChatMessageData>>)
									.ListItemsSource(&MessagesArray)
									.OnGenerateRow(this, &MSChatWidget::OnGenerateRow)
									.ScrollbarVisibility(EVisibility::Visible)
						]
					]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SBox).WidthOverride(500.0f).HeightOverride(100.0f).Padding(15.0f)
					[
						SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.VAlign(VAlign_Fill)
							.HAlign(HAlign_Fill)
							[
								SAssignNew(ChatInput, SMultiLineEditableText)
									.AutoWrapText(false)
									.WrapTextAt(500)
									.WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)
									.Text(FText::FromString(""))
									.Font(UMWidgetStyle::GetChatTextStyle())
									.HintText(LOCTEXT("Chat", "Enter a message"))
							]
					]
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SBox).WidthOverride(500.0f).Padding(15.0f)
					[
						SNew(SHorizontalBox)
							+ SHorizontalBox::Slot().VAlign(VAlign_Fill).HAlign(HAlign_Fill)
							[
								SNew(SButton).OnClicked(this, &MSChatWidget::OnSendMessage).ButtonColorAndOpacity(FLinearColor(1, 1, 1))
								[
									SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("Chat", "Send message"))
										.Justification(ETextJustify::Center).WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)
								]
							]
					]
				]
			]
		];
}

bool MSChatWidget::SupportsKeyboardFocus() const
{
	return true;
}

TSharedRef<ITableRow> MSChatWidget::OnGenerateRow(TSharedPtr<FChatMessageData> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return
		SNew(STableRow<TSharedPtr<FChatMessageData>>, OwnerTable)
		[
			SNew(MSMessageWidget).MessageData(Item.Get())
		];
}

void MSChatWidget::UpdateChatBox()
{
	if (UMPlayerInfoSubsystem* PlayerInfoManager = OwnerHUD->GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
	{
		FChatMessageData* MessageData = PlayerInfoManager->GetLastMessageData();
		
		if (MessageData)
		{
			FString Time; int Index;
			MessageData->DateTime.FindChar(' ', Index);
			MessageData->DateTime.RemoveAt(0, Index);

			MessagesArray.Add(MakeShareable(MessageData));
			ListViewWidget->RequestListRefresh(); 
			ListViewWidget->ScrollToBottom();
		}
	}
}

FReply MSChatWidget::OnSendMessage() const
{
	if (UMPlayerInfoSubsystem* PlayerInfoManager = OwnerHUD->GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
	{
		if (PlayerInfoManager->AddMessage(ChatInput->GetText().ToString()))
		{
			ChatInput->SetText(FText::FromString(""));
			OwnerHUD->GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
			OwnerHUD->GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
			FSlateApplication::Get().SetUserFocusToGameViewport(0, EFocusCause::SetDirectly);

			if (AMPlayerController* PlayerController = Cast<AMPlayerController>(OwnerHUD->GetOwningPlayerController()))
			{
				PlayerController->MessageSubmit();
			}
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
