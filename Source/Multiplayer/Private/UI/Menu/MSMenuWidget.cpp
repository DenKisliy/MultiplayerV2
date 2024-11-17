// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MSMenuWidget.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

#define LOCTEXT_NAMESPACE "Menu"

void MSMenuWidget::Construct(const FArguments& InArgs)
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
									SNew(STextBlock).Font(TitleTextStyle).Text(LOCTEXT("Menu", "Multiplayer")).Justification(ETextJustify::Center)
								]

								+ SVerticalBox::Slot().Padding(ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSMenuWidget::OnCreateSession)
										[
											SNew(STextBlock).Font(ButtonTextStyle).Text(LOCTEXT("Menu", "Create session")).Justification(ETextJustify::Center)
										]
								]

								+ SVerticalBox::Slot().Padding(ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSMenuWidget::OnFindSession)
										[
											SNew(STextBlock).Font(ButtonTextStyle).Text(LOCTEXT("Menu", "Find session")).Justification(ETextJustify::Center)
										]
								]

								+ SVerticalBox::Slot().Padding(ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSMenuWidget::OnBackToPreviousMenu)
										[
											SNew(STextBlock).Font(ButtonTextStyle).Text(LOCTEXT("Menu", "Back to previous menu")).Justification(ETextJustify::Center)
										]
								]
						]
				]
		];
}

bool MSMenuWidget::SupportsKeyboardFocus() const
{
	return true;
}

FReply MSMenuWidget::OnCreateSession() const
{
	OpenNextWidget(ETypeOfWidget::CreateSession);
	return FReply::Handled();
}

FReply MSMenuWidget::OnFindSession() const
{
	OpenNextWidget(ETypeOfWidget::FindSession);
	return FReply::Handled();
}

FReply MSMenuWidget::OnBackToPreviousMenu() const
{
	OpenNextWidget(ETypeOfWidget::LoginIn);

	return FReply::Handled();
}

void MSMenuWidget::OpenNextWidget(ETypeOfWidget Type) const
{
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(OwnerHUD.Get()))
	{
		HUD->ShowNextWidget(Type);
	}
}

#undef LOCTEXT_NAMESPACE
