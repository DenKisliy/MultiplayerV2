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

	if (GEngine && GEngine->GameViewport)
	{
		/*MenuWidget = SNew(MSMenuWidget).OwnerHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));*/
	
		LoginInWidget = SNew(MSLoginInWidget).OwnerHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(LoginInWidgetContainer, SWeakWidget).PossiblyNullContent(LoginInWidget.ToSharedRef()));

		/*RegistrationWidget = SNew(MSRegistrationWidget).OwnerHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(RegistrationWidgetContainer, SWeakWidget).PossiblyNullContent(RegistrationWidget.ToSharedRef()));*/
	}

	/*if (UMGameInstance* gameInstance = Cast<UMGameInstance>(GetGameInstance()))
	{
		FPlayerInfoStruct PlayerInfo = gameInstance->GetPlayerInfoFromGameInstance();
		bool bSetCharacterType = PlayerInfo.CharacterType != ETypeOfCharacter::None;
		
		if (IsValid(MainMenuStatic) && !bSetCharacterType)
		{
			GetOwningPlayerController()->SetShowMouseCursor(true);
			GetOwningPlayerController()->SetInputMode(FInputModeUIOnly());

			UMMainMenuWidget* mainMenuWidget = CreateWidget<UMMainMenuWidget>(this->GetOwningPlayerController(), MainMenuStatic);
			mainMenuWidget->AddToViewport();
		}

		if (bSetCharacterType)
		{
			if (IsValid(TimerStatic) && !IsValid(TimerWidget))
			{
				TimerWidget = CreateWidget<UMTimerWidget>(this->GetOwningPlayerController(), TimerStatic);
				TimerWidget->SetAnchorsInViewport(FAnchors(0.5f, 0.0f));
				TimerWidget->SetAlignmentInViewport(FVector2D(double(0.5), 0));
				TimerWidget->AddToViewport(2);
			}
			
			if (IsValid(AttributesGroupStatic))
			{
				AttributesGroupWidget = CreateWidget<UMAttributesGroupWidget>(this->GetOwningPlayerController(), AttributesGroupStatic);
				AttributesGroupWidget->AddToViewport();
			}
			
			if (IsValid(InformStatic))
			{
				InformWidget = CreateWidget<UMInformWidget>(this->GetOwningPlayerController(), InformStatic);
				InformWidget->AddToViewport();
			}

			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		}

		if (PlayerInfo.ResultOfGame != EResultOfGame::None && !bSetCharacterType)
		{
			UMResultGameWidget* resultGameWidget = CreateWidget<UMResultGameWidget>(this->GetOwningPlayerController(), ResultGameStatic);
			resultGameWidget->SetResultOfGame(PlayerInfo.ResultOfGame == EResultOfGame::Win);
			resultGameWidget->AddToViewport(5);
		}
	}*/
}

void AMPlayerHUD::SetDelegateForSendMessageEvent(FScriptDelegate Delegate)
{
	UMGameInstance* gameInstance = Cast<UMGameInstance>(GetGameInstance());
	if (gameInstance)
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
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(InformativeWidgetContainer, SWeakWidget).PossiblyNullContent(InformativeWidget.ToSharedRef()), 5);
	}
}

void AMPlayerHUD::ShowNextWidget(ETypeOfUIWidget TypeOfUIWidget)
{
	if (GEngine && GEngine->GameViewport)
	{
		if (TypeOfUIWidget != ETypeOfUIWidget::None)
		{
			GEngine->GameViewport->RemoveAllViewportWidgets();
		}

		switch (TypeOfUIWidget)
		{
		case ETypeOfUIWidget::LoginIn:
			{
				if (LoginInWidget.IsValid() && LoginInWidgetContainer.IsValid())
				{
					GEngine->GameViewport->AddViewportWidgetContent(LoginInWidgetContainer.ToSharedRef());
				}

				if (!LoginInWidget.IsValid() && !LoginInWidgetContainer.IsValid())
				{
					LoginInWidget = SNew(MSLoginInWidget).OwnerHUD(this);
					GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(LoginInWidgetContainer, SWeakWidget).PossiblyNullContent(LoginInWidget.ToSharedRef()));
				}
			}
			break;
		case ETypeOfUIWidget::Registration:
			{
				if (RegistrationWidget.IsValid() && RegistrationWidgetContainer.IsValid())
				{
					GEngine->GameViewport->AddViewportWidgetContent(RegistrationWidgetContainer.ToSharedRef());
				}

				if (!RegistrationWidget.IsValid() && !RegistrationWidgetContainer.IsValid())
				{
					RegistrationWidget = SNew(MSRegistrationWidget).OwnerHUD(this);
					GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(RegistrationWidgetContainer, SWeakWidget).PossiblyNullContent(RegistrationWidget.ToSharedRef()));
				}

			}
			break;
		}
	}
}

void AMPlayerHUD::CloseInformWidget()
{
	if (GEngine && GEngine->GameViewport)
	{
		if (InformativeWidgetContainer.IsValid())
		{
			GEngine->GameViewport->RemoveViewportWidgetContent(InformativeWidgetContainer.ToSharedRef());
		}
	}
}
