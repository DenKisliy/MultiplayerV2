// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inform/MSInformativeWidget.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

#define LOCTEXT_NAMESPACE "Informative"

void MSInformativeWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;
	WidgetData = InArgs._WidgetData;

	const FMargin ContentPadding = FMargin(50.0f);
	const FMargin ButtontPadding = FMargin(10.0f);

	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 40.0f;
	FSlateFontInfo TitleTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	TitleTextStyle.Size = 60.0f;

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
								SNew(SVerticalBox)
									+ SVerticalBox::Slot().AutoHeight().Padding(ButtontPadding)
										[
											SNew(STextBlock).Font(TitleTextStyle).Justification(ETextJustify::Center).Text(WidgetData.Get()->bWarning ?
												LOCTEXT("Informative", "Warning") : LOCTEXT("Informative", "Information"))
										]

									+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(ButtontPadding)
									[
										SNew(SHorizontalBox)
											+ SHorizontalBox::Slot().MaxWidth(600.0f).Padding(ButtontPadding)
											[
												SNew(SVerticalBox)
													+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center).Padding(ContentPadding)
													[
														SNew(STextBlock).Font(TitleTextStyle).Text(WidgetData.Get()->Text).Justification(ETextJustify::Center).AutoWrapText(true)
													]
											]
									]

									+ SVerticalBox::Slot().AutoHeight().Padding(ButtontPadding)
									[
										SNew(SHorizontalBox)
											+ SHorizontalBox::Slot().Padding(ButtontPadding)
											[
												SNew(SButton).OnClicked(this, &MSInformativeWidget::OnConfirm)
													[
														SNew(STextBlock).Font(ButtonTextStyle).Text(LOCTEXT("Informative", "Ok")).Justification(ETextJustify::Center)
													]
											]
									]
						]
			]
		]
	];
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
			HUD->CloseInformWidget();
		}
		else
		{
			HUD->ShowNextWidget(WidgetData.Get()->TypeOfNextWidget);
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
