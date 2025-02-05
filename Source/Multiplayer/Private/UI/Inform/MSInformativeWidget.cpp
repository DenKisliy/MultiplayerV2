// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inform/MSInformativeWidget.h"
#include "../../../Public/GameFramework/HUD/MMainMenuHUD.h"

#define LOCTEXT_NAMESPACE "Informative"

void MSInformativeWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;

	bWarning = InArgs._Warning.Get();

	bWaiting = InArgs._Waiting.Get();

	TypeOfNextWidget = InArgs._NextWidget.Get();

	TypeOfPreviousWidget = InArgs._PreviousWidget.Get();

	FSlateBrush* Style = new FSlateBrush();

	if (bWarning)
	{
		Style->TintColor = FColor::Red;
	}
	else
	{
		Style = const_cast<FSlateBrush*>(FAppStyle::Get().GetBrush("Brushes.Panel"));
	}

	ChildSlot
	[
		SNew(SVerticalBox)
			+ SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
			[
				SNew(SOverlay)
					+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
						[
							SNew(SBorder).BorderImage(Style)
							[
								SAssignNew(WidgetBox, SVerticalBox)
									+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
									[
										SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Justification(ETextJustify::Center).Text(bWarning ?
											LOCTEXT("Informative", "Warning") : LOCTEXT("Informative", "Information"))
									]

									+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(UMWidgetStyle::GetButtontPadding())
									[
										SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(InArgs._Text.Get()).Justification(ETextJustify::Center).AutoWrapText(true)
									]
							]
						]
			]
	];

	if (!bWaiting)
	{
		WidgetBox.Get()->AddSlot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
		[
			SNew(SButton).OnClicked(this, &MSInformativeWidget::OnConfirm)
				[
					SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("Informative", "Ok")).Justification(ETextJustify::Center)
				]
		];
	}

}

bool MSInformativeWidget::SupportsKeyboardFocus() const
{
	return true;
}

FReply MSInformativeWidget::OnConfirm() const
{
	if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
	{
		HUD->CloseWidget(ETypeOfWidget::Inform);

		if (bWarning)
		{
			HUD->SetFocus(TypeOfPreviousWidget);
		}
		else if (TypeOfNextWidget != ETypeOfWidget::None)
		{
			HUD->ShowWidget(TypeOfNextWidget);
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
