// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inform/MSResultGameWidget.h"
#include "../../../Public/GameFramework/HUD/MMainMenuHUD.h"

#define LOCTEXT_NAMESPACE "ResultGame"

void MSResultGameWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;
	bool bResultOfGame = InArgs._bResultOfGame;

	ChildSlot
	[
		SNew(SOverlay)
			+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
			[
				SNew(SBorder).BorderImage(FAppStyle::Get().GetBrush("Brushes.Panel"))
					[
						SNew(SVerticalBox)
							+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
							[
								SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("ResultGame", "Result of last game")).Justification(ETextJustify::Center)
							]

							+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
							[
								SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).ColorAndOpacity(bResultOfGame ? FColor::Green : FColor::Red).Text(InArgs._bResultOfGame ? FText::FromString("You won.") : FText::FromString("You losted.")).Justification(ETextJustify::Center)
							]

							
							+ SVerticalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
							[
								SNew(SButton).OnClicked(this, &MSResultGameWidget::OnConfirm)
									[
										SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("ResultGame", "Confirm")).Justification(ETextJustify::Center)
									]
							]
					]
			]
	];
}

bool MSResultGameWidget::SupportsKeyboardFocus() const
{
	return true;
}

FReply MSResultGameWidget::OnConfirm() const
{
	if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
	{
		if (UMPlayerInfoSubsystem* PlayerInfoSubsystem = OwnerHUD->GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
		{
			PlayerInfoSubsystem->ResetResultOfLastGame();
		}

		HUD->CloseWidget(ETypeOfWidget::ResultOfGame);
	}

	return FReply::Handled();
}
