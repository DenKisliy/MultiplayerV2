// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inform/MSInformativeWidget.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

#define LOCTEXT_NAMESPACE "Informative"

void MSInformativeWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;
	WidgetData = InArgs._WidgetData.Get();
	FStyleWidgetData* StyleData = new FStyleWidgetData();

	FSlateBrush* Style = new FSlateBrush();

	if (WidgetData.Get()->bWarning)
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
									+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
									[
										SNew(STextBlock).Font(StyleData->TitleTextStyle).Justification(ETextJustify::Center).Text(WidgetData.Get()->bWarning ?
											LOCTEXT("Informative", "Warning") : LOCTEXT("Informative", "Information"))
									]

									+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(StyleData->ButtontPadding)
									[
										SNew(STextBlock).Font(StyleData->TitleTextStyle).Text(WidgetData.Get()->Text).Justification(ETextJustify::Center).AutoWrapText(true)
									]
							]
						]
			]
	];

	if (!WidgetData.Get()->bWaitingWidget)
	{
		WidgetBox.Get()->AddSlot().AutoHeight().Padding(StyleData->ButtontPadding)
			[
				SNew(SButton).OnClicked(this, &MSInformativeWidget::OnConfirm)
					[
						SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("Informative", "Ok")).Justification(ETextJustify::Center)
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
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(OwnerHUD.Get()))
	{
		if (WidgetData.Get()->bWarning)
		{
			HUD->CloseWidget(ETypeOfWidget::Inform);
		}
		else
		{
			HUD->ShowNextWidget(WidgetData.Get()->TypeOfNextWidget);
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
