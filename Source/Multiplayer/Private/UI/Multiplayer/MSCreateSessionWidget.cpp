// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Multiplayer/MSCreateSessionWidget.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

#define LOCTEXT_NAMESPACE "CreateSession"

void MSCreateSessionWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;

	const FMargin ContentPadding = FMargin(100.0f);
	const FMargin ButtontPadding = FMargin(10.0f);

	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 40.0f;
	FSlateFontInfo TitleTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	TitleTextStyle.Size = 60.0f;

	ChildSlot
		[
			SNew(SOverlay)
				+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
				[
					SNew(SBorder).BorderImage(FAppStyle::Get().GetBrush("Brushes.Panel"))
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot().AutoHeight().Padding(ButtontPadding)
								[
									SNew(STextBlock).Font(TitleTextStyle).Text(LOCTEXT("CreateSession", "Create session")).Justification(ETextJustify::Center)
								]

								+ SVerticalBox::Slot().AutoHeight().Padding(ButtontPadding)
								[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot().AutoWidth().Padding(ButtontPadding)
										[
											SNew(STextBlock).Font(TitleTextStyle).Text(LOCTEXT("CreateSession", "Session name")).Justification(ETextJustify::Left)
										]

										+ SHorizontalBox::Slot().AutoWidth().Padding(ButtontPadding)
										[
											SAssignNew(SessionNameBoxPtr, SEditableTextBox).MinDesiredWidth(400.0f).Font(TitleTextStyle).HintText(LOCTEXT("CreateSession", "Session name"))
										]

								]

								+ SVerticalBox::Slot().Padding(ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSCreateSessionWidget::OnCreateSession)
										[
											SNew(STextBlock).Font(ButtonTextStyle).Text(LOCTEXT("CreateSession", "Create session")).Justification(ETextJustify::Center)
										]
								]

								+ SVerticalBox::Slot().Padding(ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSCreateSessionWidget::OnBackToPreviousMenu)
										[
											SNew(STextBlock).Font(ButtonTextStyle).Text(LOCTEXT("CreateSession", "Back to previous menu")).Justification(ETextJustify::Center)
										]
								]
						]
				]
		];
}

bool MSCreateSessionWidget::SupportsKeyboardFocus() const
{
	return true;
}

FReply MSCreateSessionWidget::OnCreateSession() const
{
	FString SessionName = SessionNameBoxPtr->GetText().ToString();

	if (!SessionName.IsEmpty())
	{
		if (UMSessionSubsystem* SessionSubsystem = OwnerHUD.Get()->GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
		{
			SetDefault();
			SessionSubsystem->CreateSession(FName(*SessionName), true, false);
		}
	}
	else
	{
		SessionNameBoxPtr->SetColorAndOpacity(FColor::Red);
	}

	return FReply::Handled();
}

FReply MSCreateSessionWidget::OnBackToPreviousMenu() const
{
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(OwnerHUD.Get()))
	{
		SetDefault();
		HUD->ShowNextWidget(ETypeOfWidget::Menu);
	}

	return FReply::Handled();
}

void MSCreateSessionWidget::SetDefault() const
{
	SessionNameBoxPtr->SetText(FText::GetEmpty());
	SessionNameBoxPtr->SetColorAndOpacity(FColor::White);
}

#undef LOCTEXT_NAMESPACE
