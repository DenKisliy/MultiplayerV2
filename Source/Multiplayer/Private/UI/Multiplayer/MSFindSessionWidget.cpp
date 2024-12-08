// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Multiplayer/MSFindSessionWidget.h"

#include "../../../Public/UI/Multiplayer/MSSessionInformationFoundWidget.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

#define LOCTEXT_NAMESPACE "FindSession"

void MSFindSessionWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;
	FStyleWidgetData* StyleData = new FStyleWidgetData();

	ChildSlot
	[
		SNew(SOverlay)
			+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
			[
				SNew(SBorder).BorderImage(FAppStyle::Get().GetBrush("Brushes.Recessed"))
					[
						SAssignNew(SessionsButtonsBox, SVerticalBox)
							+ SVerticalBox::Slot().AutoHeight().Padding(StyleData->ButtontPadding)
							[
								SNew(STextBlock).Font(StyleData->TitleTextStyle).Text(LOCTEXT("FindSession", "Find sessions")).Justification(ETextJustify::Center)
							]
					]
			]
	];

	AddButtonsForSessions(InArgs._FindSessions.Get());
}

bool MSFindSessionWidget::SupportsKeyboardFocus() const
{
	return true;
}

void MSFindSessionWidget::AddButtonsForSessions(TArray<FOnlineSessionSearchResult> FindSessionsData)
{
	FStyleWidgetData* StyleData = new FStyleWidgetData();

	for (FOnlineSessionSearchResult Session : FindSessionsData)
	{
		SessionsButtonsBox->AddSlot().AutoHeight().Padding(FMargin(10.0f))
		[
			SNew(MSSessionInformationFoundWidget).OwnerHUD(OwnerHUD).SessionName(GetSessionName(Session))
		];
	}

	SessionsButtonsBox->AddSlot().AutoHeight().Padding(FMargin(10.0f))
	[
		SNew(SButton).OnClicked(this, &MSFindSessionWidget::OnCloseWindow)
		[
			SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(LOCTEXT("Menu", "Close window")).Justification(ETextJustify::Center)
		]
	];
}

FReply MSFindSessionWidget::OnCloseWindow() const
{
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(OwnerHUD.Get()))
	{
		HUD->CloseWidget(ETypeOfWidget::FindSession);
	}

	return FReply::Handled();
}

FString MSFindSessionWidget::GetSessionName(FOnlineSessionSearchResult& SessionData)
{
	FString SessionName = "";
	SessionData.Session.SessionSettings.Get(FName("SESSION_NAME_KEY"), SessionName);

	return SessionName;
}
