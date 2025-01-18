// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Multiplayer/MSSessionInformationFoundWidget.h"
#include "../../../Public/GameFramework/HUD/MMainMenuHUD.h"
#include "../../../Public/GameFramework/MPlayerHUD.h"

void MSSessionInformationFoundWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwnerHUD = InArgs._OwnerHUD;
	SessionName = InArgs._SessionName.Get();

	ChildSlot
	[
		SNew(SOverlay)
			+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
			[
				SNew(SButton).OnClicked(this, &MSSessionInformationFoundWidget::OnJoinSession)
				[
					SNew(STextBlock).Font(UMWidgetStyle::GetButtonTextStyle()).Text(FText::FromString(SessionName)).Justification(ETextJustify::Center)
				]
			]
	];
}

FReply MSSessionInformationFoundWidget::OnJoinSession() const
{
	if (!SessionName.IsEmpty())
	{
		ShowInformWidget(FText::FromString("Please wait. Session connection in progress."));
	}

	return FReply::Handled();
}

void MSSessionInformationFoundWidget::ShowInformWidget(FText Text) const
{
	if (GEngine && GEngine->GameViewport)
	{
		if (AMMainMenuHUD* HUD = Cast<AMMainMenuHUD>(OwnerHUD.Get()))
		{
			FInformWidgetData InformWidgetData = FInformWidgetData(Text, false, true);
			HUD->ShowInformWidget(InformWidgetData);

			FSlateApplication::Get().SetUserFocusToGameViewport(0, EFocusCause::SetDirectly);

			FTimerHandle JoinSessionTimer;
				HUD->GetWorld()->GetTimerManager().SetTimer(JoinSessionTimer, [this]()
					{
						if (UMSessionSubsystem* SessionManager = OwnerHUD->GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
						{
							SessionManager->ConnectToSession(SessionName);
						}
					}, 1.0f, false);
		}
	}
}

#undef LOCTEXT_NAMESPACE
