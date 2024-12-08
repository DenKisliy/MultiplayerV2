// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MSMenuWidget.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

#define LOCTEXT_NAMESPACE "Menu"

void MSMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;

	FStyleWidgetData* StyleData = new FStyleWidgetData();

	ChildSlot
		[
			SNew(SOverlay)
				+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
				[
					SNew(SBorder).BorderImage(FAppStyle::Get().GetBrush("Brushes.Panel"))
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
								[
									SNew(STextBlock).Font(StyleData->TitleTextStyle).Text(LOCTEXT("Menu", "Multiplayer")).Justification(ETextJustify::Center)
								]

								+ SVerticalBox::Slot().Padding(StyleData->ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSMenuWidget::OnCreateSession)
										[
											SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("Menu", "Create session")).Justification(ETextJustify::Center)
										]
								]

								+ SVerticalBox::Slot().Padding(StyleData->ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSMenuWidget::OnFindSession)
										[
											SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("Menu", "Find session")).Justification(ETextJustify::Center)
										]
								]

								+ SVerticalBox::Slot().Padding(StyleData->ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSMenuWidget::OnBackToPreviousMenu)
										[
											SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("Menu", "Back to previous menu")).Justification(ETextJustify::Center)
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
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(OwnerHUD.Get()))
	{
		ShowInformWidget(new FInformativeWidgetData(FText::FromString("Please wait. Searching for sessions in progress."), false, true, ETypeOfWidget::FindSession));
	}

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

void MSMenuWidget::ShowInformWidget(FInformativeWidgetData* InformWidgetData) const
{
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(OwnerHUD.Get()))
	{
		HUD->ShowInformWidget(InformWidgetData);
	}
}

#undef LOCTEXT_NAMESPACE
