// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MSGameTypeMenuWidget.h"
#include "SlateOptMacros.h"
#include "../../../Public/GameFramework/HUD/MMainMenuHUD.h"
#include "../../../Public/GameFramework/MGameMode.h"

#define LOCTEXT_NAMESPACE "GameTypeMenu"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void MSGameTypeMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;

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
									SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("GameTypeMenu", "Game type menu")).Justification(ETextJustify::Center)
								]

								+ SVerticalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
								[
									SNew(SButton).OnClicked(this, &MSGameTypeMenuWidget::OnLaunchStandAloneMode)
										[
											SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("GameTypeMenu", "Launch stand alone mode")).Justification(ETextJustify::Center)
										]
								]

								+ SVerticalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
								[
									SNew(SButton).OnClicked(this, &MSGameTypeMenuWidget::OnGoToNextMultiplayerMenu)
										[
											SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("GameTypeMenu", "Go to next multiplayer menu")).Justification(ETextJustify::Center)
										]
								]

								+ SVerticalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
								[
									SNew(SButton).OnClicked(this, &MSGameTypeMenuWidget::OnBackToPreviousMenu)
										[
											SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("GameTypeMenu", "Back to previous menu")).Justification(ETextJustify::Center)
										]
								]
						]
				]
		];

}

bool MSGameTypeMenuWidget::SupportsKeyboardFocus() const
{
	return true;
}

FReply MSGameTypeMenuWidget::OnLaunchStandAloneMode() const
{
	if (IsValid(OwnerHUD->GetWorld()))
	{
		if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(OwnerHUD->GetWorld())))
		{
			OwnerHUD->GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
			UGameplayStatics::OpenLevel(OwnerHUD->GetWorld(), FName(*GameMode->StandAloneLevelURL), true);
		}
	}

	return FReply::Handled();
}

FReply MSGameTypeMenuWidget::OnGoToNextMultiplayerMenu() const
{
	OpenNextWidget(ETypeOfWidget::MultiplayerMenu);

	return FReply::Handled();
}

FReply MSGameTypeMenuWidget::OnBackToPreviousMenu() const
{
	OpenNextWidget(ETypeOfWidget::LoginIn);

	return FReply::Handled();
}

void MSGameTypeMenuWidget::OpenNextWidget(ETypeOfWidget Type) const
{
	if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
	{
		HUD->ShowWidget(Type);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE