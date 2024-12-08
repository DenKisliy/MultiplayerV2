// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Multiplayer/MSSessionInformationFoundWidget.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

void MSSessionInformationFoundWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;
	SessionName = InArgs._SessionName.Get();

	FStyleWidgetData* StyleData = new FStyleWidgetData();

	ChildSlot
	[
		SNew(SOverlay)
			+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
			[
				SNew(SButton).OnClicked(this, &MSSessionInformationFoundWidget::OnJoinSession)
				[
					SNew(STextBlock).Font(StyleData->ButtonTextStyle).Text(FText::FromString(SessionName)).Justification(ETextJustify::Center)
				]
			]
	];
}

FReply MSSessionInformationFoundWidget::OnJoinSession() const
{
	if (!SessionName.IsEmpty())
	{
		if (UMSessionSubsystem* SessionManager = OwnerHUD->GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
		{
			SessionManager->SetJoinSessionName(SessionName);
			ShowInformWidget(new FInformativeWidgetData(FText::FromString("Please wait. Session connection in progress."), false, true, ETypeOfWidget::JoinSession));
		}
	}

	return FReply::Handled();
}

void MSSessionInformationFoundWidget::ShowInformWidget(FInformativeWidgetData* InformWidgetData) const
{
	if (AMPlayerHUD* HUD = Cast<AMPlayerHUD>(OwnerHUD))
	{
		HUD->ShowInformWidget(InformWidgetData);
	}
}

#undef LOCTEXT_NAMESPACE
