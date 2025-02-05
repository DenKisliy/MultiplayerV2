// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/HUD/MPlayingHUD.h"
#include "GameFramework/MGameMode.h"
#include "Kismet/GameplayStatics.h"

AMPlayingHUD::AMPlayingHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMPlayingHUD::SetTimeTimerWidget(bool bMain, int Time)
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

void AMPlayingHUD::SetValueForAttribute(EAttributeType Type, float Value)
{
	if (IsValid(AttributesGroupWidget))
	{
		AttributesGroupWidget->SetValueForAttribute(Type, Value);
	}
}

void AMPlayingHUD::SetRemoveOrAddAttributesGroupWidget(bool bAdd)
{
	if (!IsValid(AttributesGroupWidget) && IsValid(AttributesGroupStatic))
	{
		AttributesGroupWidget = CreateWidget<UMAttributesGroupWidget>(this->GetOwningPlayerController(), AttributesGroupStatic);
	}

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

void AMPlayingHUD::ShowInventory(bool bValue)
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

void AMPlayingHUD::ShowInformText(FString Text)
{
	if (IsValid(InformWidget))
	{
		InformWidget->SetText(Text);
	}
}

void AMPlayingHUD::CreateChat()
{
	if (!IsValid(ChatWidget) && IsValid(ChatStatic))
	{
		ChatWidget = CreateWidget(GetWorld()->GetFirstPlayerController(), ChatStatic);
		ChatWidget->AddToViewport();
	}

}

void AMPlayingHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(GetWorld()) && IsValid(GetWorld()->GetFirstPlayerController()))
	{
		bool bStandAloneMode = false;

		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		FSlateApplication::Get().SetUserFocusToGameViewport(0, EFocusCause::SetDirectly);

		if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			bStandAloneMode = GameMode->IsStandAloneMode();
		}
	}

	SetRemoveOrAddAttributesGroupWidget(true);
}

void AMPlayingHUD::DrawHUD()
{
	Super::DrawHUD();

	if (IsValid(GetOwningPlayerController()))
	{
		if (GetOwningPlayerController()->WasInputKeyJustPressed(EKeys::NumPadOne))
		{
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			ChatWidget->SetFocus();
		}
	}
}
