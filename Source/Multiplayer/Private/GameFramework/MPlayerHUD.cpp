// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/MPlayerHUD.h"

AMPlayerHUD::AMPlayerHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMPlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>()->IsUserSignIn())
	{
		if (GEngine && GEngine->GameViewport)
		{
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());

			LoginInWidget = SNew(MSLoginInWidget).OwnerHUD(this);
			GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(LoginInContainer, SWeakWidget).PossiblyNullContent(LoginInWidget.ToSharedRef()));
		}
	}
}

void AMPlayerHUD::SetDelegateForSendMessageEvent(FScriptDelegate Delegate)
{
	if (UMGameInstance* gameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		if (gameInstance->GetPlayerInfoFromGameInstance().CharacterType != ETypeOfCharacter::None)
		{
			if (!IsValid(ChatPanelWidget) && IsValid(ChatPanelStatic))
			{
				ChatPanelWidget = CreateWidget<UMChatPanelWidget>(this->GetOwningPlayerController(), ChatPanelStatic);
				if (!ChatPanelWidget->IsInViewport())
				{
					ChatPanelWidget->AddToViewport();
				}
			}

			if (IsValid(ChatPanelWidget))
			{
				ChatPanelWidget->SendMessageDelegate.Clear();
				ChatPanelWidget->SendMessageDelegate.Add(Delegate);
			}
		}
	}
}

void AMPlayerHUD::AddMessage(FString PlayerName, FString MessageText)
{
	if (IsValid(ChatPanelWidget))
	{
		ChatPanelWidget->AddMessage(PlayerName, MessageText);
	}
}

void AMPlayerHUD::SetTimeTimerWidget(bool bMain, int Time)
{
	if (!IsValid(TimerWidget))
	{
		TimerWidget = CreateWidget<UMTimerWidget>(this->GetOwningPlayerController(), TimerStatic);
		TimerWidget->SetAnchorsInViewport(FAnchors(0.5f, 0.0f));
		TimerWidget->SetAlignmentInViewport(FVector2D(double(0.5), 0));
		TimerWidget->AddToViewport(2);
	}

	TimerWidget->SetTimeForTimer(bMain, Time);
}

void AMPlayerHUD::SetValueForAttribute(EAttributeType Type, float Value)
{
	if (IsValid(AttributesGroupWidget))
	{
		AttributesGroupWidget->SetValueForAttribute(Type, Value);
	}
}

void AMPlayerHUD::SetRemoveOrAddAttributesGroupWidget(bool bAdd)
{
	if (IsValid(AttributesGroupWidget))
	{
		if (bAdd)
		{
			if (!AttributesGroupWidget->IsInViewport() && AttributesGroupWidget->GetParent())
			{
				AttributesGroupWidget->AddToViewport();
			}
			else
			{
				if (IsValid(AttributesGroupStatic) && !AttributesGroupWidget->IsInViewport())
				{
					AttributesGroupWidget = CreateWidget<UMAttributesGroupWidget>(this->GetOwningPlayerController(), AttributesGroupStatic);
					AttributesGroupWidget->AddToViewport();
				}
			}
		}
		else
		{
			if (AttributesGroupWidget->IsInViewport())
			{
				AttributesGroupWidget->RemoveFromParent();
			}
		}
	}
}

void AMPlayerHUD::ShowInventory(bool bValue)
{
	if (bValue)
	{
		if (IsValid(InventoryStatic))
		{
			if (!IsValid(InventoryWidget))
			{
				InventoryWidget = CreateWidget<UMInventoryWidget>(this->GetOwningPlayerController(), InventoryStatic);
				InventoryWidget->AddToViewport(-1);
			}
			else
			{
				InventoryWidget->RemoveFromParent();
				InventoryWidget = nullptr;
			}
		}
	}
	else
	{
		if (IsValid(InventoryWidget))
		{
			InventoryWidget->RemoveFromParent();
			InventoryWidget = nullptr;
		}
	}
}

void AMPlayerHUD::ShowInformText(FString Text)
{
	if (IsValid(InformWidget))
	{
		InformWidget->SetText(Text);
	}
}

void AMPlayerHUD::ShowInformWidget(FInformativeWidgetData* InformWidgetData)
{
	if (GEngine && GEngine->GameViewport)
	{
		InformativeWidget = SNew(MSInformativeWidget).OwnerHUD(this).WidgetData(InformWidgetData);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(InformativeContainer, SWeakWidget).PossiblyNullContent(InformativeWidget.ToSharedRef()), 5);

		if (InformWidgetData->bWaitingWidget)
		{
			FTimerHandle Timer;
			CurrentWidgetType = InformWidgetData->TypeOfNextWidget;
			GetWorld()->GetTimerManager().SetTimer(Timer, this, &AMPlayerHUD::GoToNextWidgetStep, 1.0f, false);
		}
	}
}

void AMPlayerHUD::ShowNextWidget(ETypeOfWidget TypeOfUIWidget)
{
	if (GEngine && GEngine->GameViewport)
	{
		if (TypeOfUIWidget != ETypeOfWidget::None)
		{
			GEngine->GameViewport->RemoveAllViewportWidgets();
		}

		switch (TypeOfUIWidget)
		{
		case ETypeOfWidget::LoginIn:
			{
				if (LoginInWidget.IsValid() && LoginInContainer.IsValid())
				{
					GEngine->GameViewport->AddViewportWidgetContent(LoginInContainer.ToSharedRef());
				}

				if (!LoginInWidget.IsValid() && !LoginInContainer.IsValid())
				{
					LoginInWidget = SNew(MSLoginInWidget).OwnerHUD(this);
					GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(LoginInContainer, SWeakWidget).PossiblyNullContent(LoginInWidget.ToSharedRef()));
				}
			}
			break;
		case ETypeOfWidget::Registration:
			{
				if (RegistrationWidget.IsValid() && RegistrationContainer.IsValid())
				{
					GEngine->GameViewport->AddViewportWidgetContent(RegistrationContainer.ToSharedRef());
				}

				if (!RegistrationWidget.IsValid() && !RegistrationContainer.IsValid())
				{
					RegistrationWidget = SNew(MSRegistrationWidget).OwnerHUD(this);
					GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(RegistrationContainer, SWeakWidget).PossiblyNullContent(RegistrationWidget.ToSharedRef()));
				}

			}
			break;
		case ETypeOfWidget::Menu:
			{
				if (MenuWidget.IsValid() && MenuContainer.IsValid())
				{
					GEngine->GameViewport->AddViewportWidgetContent(MenuContainer.ToSharedRef());
				}

				if (!MenuWidget.IsValid() && !MenuContainer.IsValid())
				{
					MenuWidget = SNew(MSMenuWidget).OwnerHUD(this);
					GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuContainer, SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));
				}
			}
			break;
		case ETypeOfWidget::CreateSession:
		{
			if (CreateSessionWidget.IsValid() && CreateSessionContainer.IsValid())
			{
				GEngine->GameViewport->AddViewportWidgetContent(CreateSessionContainer.ToSharedRef());
			}

			if (!CreateSessionWidget.IsValid() && !CreateSessionContainer.IsValid())
			{
				CreateSessionWidget = SNew(MSCreateSessionWidget).OwnerHUD(this);
				GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(CreateSessionContainer, SWeakWidget).PossiblyNullContent(CreateSessionWidget.ToSharedRef()));
			}
		}
		break;
		}
	}
}

void AMPlayerHUD::OnFindSessions(bool bFindSession)
{
	if (bFindSession)
	{
		if (UMSessionSubsystem* sessionManager = GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
		{
			if (sessionManager->GetFindSessionsNamesArray().Num() > 0)
			{
				if (GEngine && GEngine->GameViewport)
				{
					CloseWidget(ETypeOfWidget::Inform);
					FindSessionWidget = SNew(MSFindSessionWidget).OwnerHUD(this).FindSessions(sessionManager->GetFindSessionsNamesArray());
					GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(FindSessionContainer, SWeakWidget).PossiblyNullContent(FindSessionWidget.ToSharedRef()), 5);
				}
			}
		}
	}
}

void AMPlayerHUD::GoToNextWidgetStep()
{
	switch (CurrentWidgetType)
	{
	case ETypeOfWidget::CreateSession:
		if (CreateSessionContainer.IsValid())
		{
			CreateSessionWidget.Get()->CreateSession();
		}
		break;
	case ETypeOfWidget::FindSession:
		if (MenuContainer.IsValid())
		{
			if (UMSessionSubsystem* SessionManager = GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
			{
				SessionManager->ResultOfFindSessionsDelegate.AddDynamic(this, &AMPlayerHUD::OnFindSessions);
				SessionManager->FindSessions();
			}
		}
		break;
	case ETypeOfWidget::JoinSession:
		if (FindSessionContainer.IsValid())
		{
			if (UMSessionSubsystem* SessionManager = GetGameInstance()->GetSubsystem<UMSessionSubsystem>())
			{
				SessionManager->ConnectToSession();
			}
		}
		break;
	}

	CurrentWidgetType = ETypeOfWidget::None;
}

void AMPlayerHUD::CloseWidget(ETypeOfWidget TypeOfWidget)
{
	if (GEngine && GEngine->GameViewport)
	{
		switch (TypeOfWidget)
		{
		case ETypeOfWidget::FindSession:
			if (FindSessionContainer.IsValid())
			{
				GEngine->GameViewport->RemoveViewportWidgetContent(FindSessionContainer.ToSharedRef());
			}
			break;
		case ETypeOfWidget::Inform:
			if (InformativeContainer.IsValid())
			{
				GEngine->GameViewport->RemoveViewportWidgetContent(InformativeContainer.ToSharedRef());
			}
			break;
		}
	}
}
