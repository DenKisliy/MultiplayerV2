// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/HUD/MPlayingHUD.h"
#include "GameFramework/MGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../../../Public/GameFramework/MPlayerState.h"
#include "../../../Public/Subsystem/MPlayerInfoSubsystem.h"

AMPlayingHUD::AMPlayingHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMPlayingHUD::SetTimeTimerWidget(bool bMain, int Time)
{
	if (!IsValid(TimerWidget) && IsValid(TimerStatic))
	{
		TimerWidget = CreateWidget<UMTimerWidget>(this->GetOwningPlayerController(), TimerStatic);
		TimerWidget->SetAnchorsInViewport(FAnchors(0.5f, 0.0f));
		TimerWidget->SetAlignmentInViewport(FVector2D(double(0.5), 0));
	}

	if(!TimerWidget->IsInViewport())
	{
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

void AMPlayingHUD::ShowInventory()
{
	if (!IsValid(InventoryWidget))
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
		InventoryWidget->RemoveFromParent();
		InventoryWidget = nullptr;
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
	if (!IsStandAloneMode())
	{
		if (!IsValid(ChatWidget) && IsValid(ChatStatic))
		{
			ChatWidget = CreateWidget(GetWorld()->GetFirstPlayerController(), ChatStatic);
			ChatWidget->AddToViewport();
		}
	}
}

void AMPlayingHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UpdatePlyerName();

	if (IsValid(GetWorld()) && IsValid(GetWorld()->GetFirstPlayerController()))
	{
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		FSlateApplication::Get().SetUserFocusToGameViewport(0, EFocusCause::SetDirectly);
	}

	SetRemoveOrAddAttributesGroupWidget(true);
	CreateChat();
}

void AMPlayingHUD::DrawHUD()
{
	Super::DrawHUD();

	if (IsValid(GetOwningPlayerController()))
	{
		if (GetOwningPlayerController()->WasInputKeyJustPressed(EKeys::NumPadZero))
		{
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			ChatWidget->SetFocus();
		}
		if (GetOwningPlayerController()->WasInputKeyJustPressed(EKeys::NumPadOne))
		{
			ShowInventory();
		}
	}
}

bool AMPlayingHUD::IsStandAloneMode()
{
	if (AMGameMode* GameMode = Cast<AMGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		return GameMode->IsStandAloneMode();
	}

	return false;
}

void AMPlayingHUD::UpdatePlyerName()
{
	if (!IsStandAloneMode())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		if (AMPlayerState* PS = Cast<AMPlayerState>(GetOwningPlayerController()->PlayerState))
		{
			if (!PS->IsUserNameByLogin())
			{
				if (UMPlayerInfoSubsystem* PlayerInfoManager = this->GetGameInstance()->GetSubsystem<UMPlayerInfoSubsystem>())
				{
					PS->UpdateUserNameByLogin(PlayerInfoManager->GetLoginOfUser());
					return;
				}
			}
		}

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMPlayingHUD::UpdatePlyerName, 1.0f, false);
	}
}
