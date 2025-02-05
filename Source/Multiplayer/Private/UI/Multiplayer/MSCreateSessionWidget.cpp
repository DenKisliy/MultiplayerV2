// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Multiplayer/MSCreateSessionWidget.h"
#include "../../../Public/GameFramework/HUD/MMainMenuHUD.h"

#define LOCTEXT_NAMESPACE "CreateSession"

void MSCreateSessionWidget::Construct(const FArguments& InArgs)
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
									SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("CreateSession", "Create session")).Justification(ETextJustify::Center)
								]

								+ SVerticalBox::Slot().AutoHeight().Padding(UMWidgetStyle::GetButtontPadding())
								[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot().AutoWidth().Padding(UMWidgetStyle::GetButtontPadding())
										[
											SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("CreateSession", "Session name")).Justification(ETextJustify::Left)
										]

										+ SHorizontalBox::Slot().AutoWidth().Padding(UMWidgetStyle::GetButtontPadding())
										[
											SAssignNew(SessionNameBoxPtr, SEditableTextBox).MinDesiredWidth(400.0f).ClearKeyboardFocusOnCommit(true)
												.Font(UMWidgetStyle::GetTitleTextStyle()).HintText(LOCTEXT("CreateSession", "Session name"))
										]

								]

								+ SVerticalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
								[
									SNew(SButton).OnClicked(this, &MSCreateSessionWidget::OnCreateSession)
										[
											SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("CreateSession", "Create session")).Justification(ETextJustify::Center)
										]
								]

								+ SVerticalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
								[
									SNew(SButton).OnClicked(this, &MSCreateSessionWidget::OnBackToPreviousMenu)
										[
											SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("CreateSession", "Back to previous menu")).Justification(ETextJustify::Center)
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

void MSCreateSessionWidget::CreateSession() const
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
		ShowInformWidget(FText::FromString("Please wait. Session creation in progress."));
	}
	else
	{
		ShowInformWidget(FText::FromString("Session name is not set."), true, false, ETypeOfWidget::CreateSession);
		SessionNameBoxPtr->SetColorAndOpacity(FColor::Red);
	}

	return FReply::Handled();
}

FReply MSCreateSessionWidget::OnBackToPreviousMenu() const
{
	if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
	{
		SetDefault();
		HUD->ShowWidget(ETypeOfWidget::MultiplayerMenu);
	}

	return FReply::Handled();
}

void MSCreateSessionWidget::SetDefault() const
{
	SessionNameBoxPtr->SetText(FText::GetEmpty());
	SessionNameBoxPtr->SetColorAndOpacity(FColor::White);
}


void MSCreateSessionWidget::ShowInformWidget(FText Text, bool bWarning, bool bWaiting, ETypeOfWidget PreviousWidget) const
{
	if (GEngine && GEngine->GameViewport)
	{
		if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
		{
			FInformWidgetData InformWidgetData = FInformWidgetData(Text, bWarning, bWaiting,
				ETypeOfWidget::None, PreviousWidget);
			HUD->ShowInformWidget(InformWidgetData);
			
			if (bWaiting)
			{
				FSlateApplication::Get().SetUserFocusToGameViewport(0, EFocusCause::SetDirectly);

				FTimerHandle CreateSessionTimer;
				HUD->GetWorld()->GetTimerManager().SetTimer(CreateSessionTimer, [this]()
					{
						CreateSession();
					}, 1.0f, false);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
