// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/MSMultiplayerMenuWidget.h"
#include "../../../Public/GameFramework/HUD/MMainMenuHUD.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

#define LOCTEXT_NAMESPACE "MultiplayerMenu"

void MSMultiplayerMenuWidget::Construct(const FArguments& InArgs)
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
								SNew(STextBlock).Font(UMWidgetStyle::GetTitleTextStyle()).Text(LOCTEXT("MultiplayerMenu", "Multiplayer menu")).Justification(ETextJustify::Center)
							]

							+ SVerticalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
							[
								SNew(SButton).OnClicked(this, &MSMultiplayerMenuWidget::OnCreateSession)
									[
										SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("MultiplayerMenu", "Create session")).Justification(ETextJustify::Center)
									]
							]

							+ SVerticalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
							[
								SNew(SButton).OnClicked(this, &MSMultiplayerMenuWidget::OnFindSession)
									[
										SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("MultiplayerMenu", "Find session")).Justification(ETextJustify::Center)
									]
							]

							+ SVerticalBox::Slot().Padding(UMWidgetStyle::GetButtontPadding())
							[
								SNew(SButton).OnClicked(this, &MSMultiplayerMenuWidget::OnBackToPreviousMenu)
									[
										SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(LOCTEXT("MultiplayerMenu", "Back to previous menu")).Justification(ETextJustify::Center)
									]
							]
					]
			]
	];
}

bool MSMultiplayerMenuWidget::SupportsKeyboardFocus() const
{
	return true;
}

FReply MSMultiplayerMenuWidget::OnCreateSession() const
{
	OpenNextWidget(ETypeOfWidget::CreateSession);
	return FReply::Handled();
}

FReply MSMultiplayerMenuWidget::OnFindSession() const
{
	if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
	{
		ShowInformWidget(FText::FromString("Please wait. Searching for sessions in progress."));
	}

	return FReply::Handled();
}

FReply MSMultiplayerMenuWidget::OnBackToPreviousMenu() const
{
	OpenNextWidget(ETypeOfWidget::GameTypeMenu);

	return FReply::Handled();
}

void MSMultiplayerMenuWidget::OpenNextWidget(ETypeOfWidget Type) const
{
	if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
	{
		HUD->ShowWidget(Type);
	}
}

void MSMultiplayerMenuWidget::ShowInformWidget(FText Text, bool bWarning, bool bWaiting, ETypeOfWidget PreviousWidget) const
{
	if (GEngine && GEngine->GameViewport)
	{
		if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
		{
			FInformWidgetData InformWidgetData = FInformWidgetData(Text, bWarning, bWaiting,
				ETypeOfWidget::None, PreviousWidget);
			HUD->ShowInformWidget(InformWidgetData);
			
			FSlateApplication::Get().SetUserFocusToGameViewport(0, EFocusCause::SetDirectly);
			
			if (bWaiting)
			{
				FTimerHandle FindSessionTimer;
				HUD->GetWorld()->GetTimerManager().SetTimer(FindSessionTimer, [this]()
					{
						FindSessions();
					}, 1.0f, false);
			}
		}
	}
}

void MSMultiplayerMenuWidget::FindSessions() const
{
	if (UMSessionSubsystem* SessionSubsystem = OwnerHUD.Get()->GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
	{
		SessionSubsystem->ResultOfFindSessionsDelegate.BindRaw(this, &MSMultiplayerMenuWidget::FindSessionsResult);
		SessionSubsystem->FindSessions();
	}
}

void MSMultiplayerMenuWidget::FindSessionsResult(TArray<FOnlineSessionSearchResult> FindSessions) const
{
	if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
	{
		if (IsValid(HUD->GetWorld()))
		{
			if (HUD->GetWorld()->GetGameViewport())
			{
				HUD->CloseWidget(ETypeOfWidget::Inform);

				if (FindSessions.Num() > 0)
				{
					HUD->FindSessionWidget = SNew(MSFindSessionWidget).OwnerHUD(HUD).FindSessions(FindSessions);
					HUD->GetWorld()->GetGameViewport()->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(HUD->FindSessionWidget.ToSharedRef()), 5);
				}
				else
				{
					ShowInformWidget(FText::FromString("Could not find existing sessions."), true, false, ETypeOfWidget::MultiplayerMenu);
				}
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
