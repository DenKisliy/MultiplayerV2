// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Multiplayer/MSCreateSessionWidget.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

#define LOCTEXT_NAMESPACE "CreateSession"

void MSCreateSessionWidget::Construct(const FArguments& InArgs)
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
									SNew(STextBlock).Font(StyleData->TitleTextStyle).Text(LOCTEXT("CreateSession", "Create session")).Justification(ETextJustify::Center)
								]

								+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
								[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot().AutoWidth().Padding(StyleData->ButtontPadding)
										[
											SNew(STextBlock).Font(StyleData->TitleTextStyle).Text(LOCTEXT("CreateSession", "Session name")).Justification(ETextJustify::Left)
										]

										+ SHorizontalBox::Slot().AutoWidth().Padding(StyleData->ButtontPadding)
										[
											SAssignNew(SessionNameBoxPtr, SEditableTextBox).MinDesiredWidth(400.0f).Font(StyleData->TitleTextStyle).HintText(LOCTEXT("CreateSession", "Session name"))
										]

								]

								+ SVerticalBox::Slot().Padding(StyleData->ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSCreateSessionWidget::OnCreateSession)
										[
											SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("CreateSession", "Create session")).Justification(ETextJustify::Center)
										]
								]

								+ SVerticalBox::Slot().Padding(StyleData->ButtontPadding)
								[
									SNew(SButton).OnClicked(this, &MSCreateSessionWidget::OnBackToPreviousMenu)
										[
											SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("CreateSession", "Back to previous menu")).Justification(ETextJustify::Center)
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

void MSCreateSessionWidget::CreateSession()
{
	FString SessionName = SessionNameBoxPtr->GetText().ToString();

	if (!SessionName.IsEmpty())
	{
		if (UMSessionSubsystem* SessionSubsystem = OwnerHUD.Get()->GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
		{
			SessionSubsystem->CreateSession(FName(*SessionName), true, false);

			SetDefault();
		}
	}
}

FReply MSCreateSessionWidget::OnCreateSession() const
{
	FString SessionName = SessionNameBoxPtr->GetText().ToString();

	if (!SessionName.IsEmpty())
	{
		ShowInformWidget(new FInformativeWidgetData(FText::FromString("Please wait. Session creation in progress."), false, true, ETypeOfWidget::CreateSession));
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


void MSCreateSessionWidget::ShowInformWidget(FInformativeWidgetData* InformWidgetData) const
{
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(OwnerHUD.Get()))
	{
		HUD->ShowInformWidget(InformWidgetData);
	}
}

#undef LOCTEXT_NAMESPACE
