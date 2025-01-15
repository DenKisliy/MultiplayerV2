// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/MPlayerHUD.h"

#include "../../Public/GameFramework/MGameMode.h"

AMPlayerHUD::AMPlayerHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMPlayerHUD::BeginPlay()
{
	Super::BeginPlay();
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

void AMPlayerHUD::ShowWidget(ETypeOfWidget TypeOfUIWidget)
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
				if (!LoginInWidget.IsValid())
				{
					LoginInWidget = SNew(MSLoginInWidget).OwnerHUD(this);
				}

				GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(LoginInWidget.ToSharedRef()));

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

				GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(RegistrationWidget.ToSharedRef()));

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

				GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MultiplayerMenuWidget.ToSharedRef()));
			}
			break;
		case ETypeOfWidget::GameTypeMenu:
			{
				if (!GameTypeMenuWidget.IsValid())
				{
					GameTypeMenuWidget = SNew(MSGameTypeMenuWidget).OwnerHUD(this);
				}

				GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameTypeMenuWidget.ToSharedRef()));
			}
			break;
		case ETypeOfWidget::CreateSession:
		{
			if (!CreateSessionWidget.IsValid())
			{
				CreateSessionWidget = SNew(MSCreateSessionWidget).OwnerHUD(this);
			}

			GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(CreateSessionWidget.ToSharedRef()));

			if (CreateSessionWidget.IsValid() && CreateSessionWidget->SessionNameBoxPtr.IsValid())
				FSlateApplication::Get().SetKeyboardFocus(CreateSessionWidget->SessionNameBoxPtr);
		}
		break;
		}
	}
}

void AMPlayerHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	bool bStandAloneMode = false;

	if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		bStandAloneMode = GameMode->IsStandAloneMode();
	}

	if (bStandAloneMode)
	{
		if (GEngine && GEngine->GameViewport)
		{
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());

			FSlateApplication::Get().SetUserFocusToGameViewport(0, EFocusCause::SetDirectly);;
		}
	}
	else
	{
		if (!GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>()->IsUserSignIn())
		{
			if (GEngine && GEngine->GameViewport)
			{
				GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
				GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());

				ShowWidget(ETypeOfWidget::LoginIn);
			}
		}
		else
		{
			if (GEngine && GEngine->GameViewport)
			{
				GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
				GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
				FSlateApplication::Get().SetUserFocusToGameViewport(0, EFocusCause::SetDirectly);
				
				ChatWidget = SNew(MSChatWidget).OwnerHUD(this);
				GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(ChatWidget.ToSharedRef()));
			}
		}
	}

}

void AMPlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	if (IsValid(GetOwningPlayerController()))
	{
		if (GetOwningPlayerController()->WasInputKeyJustPressed(EKeys::NumPadOne))
		{
			if (ChatWidget.IsValid() && ChatWidget->ChatInput.IsValid())
			{
				GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
				GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
				FSlateApplication::Get().SetKeyboardFocus(ChatWidget->ChatInput);
			}
		}
	}
}

void AMPlayerHUD::CloseWidget(ETypeOfWidget TypeOfWidget)
{
	if (GEngine && GEngine->GameViewport)
	{
		switch (TypeOfWidget)
		{
		case ETypeOfWidget::FindSession:
			if (FindSessionWidget.IsValid())
			{
				GEngine->GameViewport->RemoveViewportWidgetContent(FindSessionWidget.ToSharedRef());
			}
			break;
		case ETypeOfWidget::Inform:
			if (InformContainer.IsValid())
			{
				GEngine->GameViewport->RemoveViewportWidgetContent(InformContainer.ToSharedRef());
			}
			break;
		}
	}
}

void AMPlayerHUD::SetFocus(ETypeOfWidget TypeOfWidget)
{
	if (GEngine && GEngine->GameViewport)
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

