// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/HUD/MMainMenuHUD.h"

AMMainMenuHUD::AMMainMenuHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMMainMenuHUD::ShowWidget(ETypeOfWidget TypeOfUIWidget)
{
	if (IsValid(GetWorld()))
	{
		if (GetWorld()->GetGameViewport())
		{
			if (TypeOfUIWidget != ETypeOfWidget::None)
			{
				GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();
			}

			switch (TypeOfUIWidget)
			{
			case ETypeOfWidget::LoginIn:
			{
				if (!LoginInWidget.IsValid())
				{
					LoginInWidget = SNew(MSLoginInWidget).OwnerHUD(this);
				}

				GetWorld()->GetGameViewport()->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(LoginInWidget.ToSharedRef()));

				if (LoginInWidget.IsValid() && LoginInWidget->LoginBoxPtr.IsValid())
					FSlateApplication::Get().SetKeyboardFocus(LoginInWidget->LoginBoxPtr);
			}
			break;

			case ETypeOfWidget::Registration:
			{
				if (!RegistrationWidget.IsValid())
				{
					RegistrationWidget = SNew(MSRegistrationWidget).OwnerHUD(this);
				}

				GetWorld()->GetGameViewport()->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(RegistrationWidget.ToSharedRef()));

				if (RegistrationWidget.IsValid() && RegistrationWidget->LoginBoxPtr.IsValid())
					FSlateApplication::Get().SetKeyboardFocus(RegistrationWidget->LoginBoxPtr);

			}
			break;

			case ETypeOfWidget::MultiplayerMenu:
			{
				if (!MultiplayerMenuWidget.IsValid())
				{
					MultiplayerMenuWidget = SNew(MSMultiplayerMenuWidget).OwnerHUD(this);
				}

				GetWorld()->GetGameViewport()->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MultiplayerMenuWidget.ToSharedRef()));
			}
			break;

			case ETypeOfWidget::GameTypeMenu:
			{
				if (!GameTypeMenuWidget.IsValid())
				{
					GameTypeMenuWidget = SNew(MSGameTypeMenuWidget).OwnerHUD(this);
				}

				GetWorld()->GetGameViewport()->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameTypeMenuWidget.ToSharedRef()));
			}
			break;

			case ETypeOfWidget::CreateSession:
			{
				if (!CreateSessionWidget.IsValid())
				{
					CreateSessionWidget = SNew(MSCreateSessionWidget).OwnerHUD(this);
				}

				GetWorld()->GetGameViewport()->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(CreateSessionWidget.ToSharedRef()));

				if (CreateSessionWidget.IsValid() && CreateSessionWidget->SessionNameBoxPtr.IsValid())
					FSlateApplication::Get().SetKeyboardFocus(CreateSessionWidget->SessionNameBoxPtr);
			}
			break;
			}
		}
	}
}

void AMMainMenuHUD::CloseWidget(ETypeOfWidget TypeOfWidget)
{
	if (IsValid(GetWorld()))
	{
		if (GetWorld()->GetGameViewport())
		{
			switch (TypeOfWidget)
			{
			case ETypeOfWidget::FindSession:
				if (FindSessionWidget.IsValid())
				{
					GetWorld()->GetGameViewport()->RemoveViewportWidgetContent(FindSessionWidget.ToSharedRef());
				}
				break;
			case ETypeOfWidget::Inform:
				if (InformContainer.IsValid())
				{
					GetWorld()->GetGameViewport()->RemoveViewportWidgetContent(InformContainer.ToSharedRef());
				}
				break;
			}
		}
	}
}

void AMMainMenuHUD::SetFocus(ETypeOfWidget TypeOfWidget)
{
	if (IsValid(GetWorld()))
	{
		if (GetWorld()->GetGameViewport())
		{
			switch (TypeOfWidget)
			{
			case ETypeOfWidget::LoginIn:
			{
				if (LoginInWidget.IsValid())
					LoginInWidget->SetFocus();
			}
			break;
			case ETypeOfWidget::Registration:
			{
				if (RegistrationWidget.IsValid())
					RegistrationWidget->SetFocus();

			}
			break;

			case ETypeOfWidget::CreateSession:
			{
				if (CreateSessionWidget.IsValid() && CreateSessionWidget->SessionNameBoxPtr.IsValid())
					FSlateApplication::Get().SetKeyboardFocus(CreateSessionWidget->SessionNameBoxPtr);
			}
			break;
			}
		}
	}
}

void AMMainMenuHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(GetWorld()))
	{
		if (GetWorld()->GetGameViewport())
		{
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());

			ShowWidget(ETypeOfWidget::LoginIn);
		}
	}
}
