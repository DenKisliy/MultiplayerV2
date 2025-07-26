// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerWidget/Item/MInventoryWidget.h"
#include "../../../../Public/Character/MPlayerCharacter.h"

void UMInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateItemsList();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetOwningPlayer()))
	{
		if (AMPlayerCharacter* Player = Cast<AMPlayerCharacter>(PlayerController->GetPawn()))
		{
			Player->InventoryComponent->UpdateInventoryDelegate.AddDynamic(this, &UMInventoryWidget::CreateItemsList);
		}
	}
}

void UMInventoryWidget::CreateItemsList()
{
	ScrollBox->ClearChildren();
	TArray<UMItemBase*> ItemsArray;
	if (APlayerController* PlayerController = Cast<APlayerController>(GetOwningPlayer()))
	{
		if (AMPlayerCharacter* Player = Cast<AMPlayerCharacter>(PlayerController->GetPawn()))
		{
			ItemsArray = Player->InventoryComponent->GetItemsArray();
		}
	}

	if (ItemsArray.Num() > 0)
	{
		InformTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		int Count = 0;
		UHorizontalBox* HorizontalBox = nullptr;
		ScrollBox->SetAlwaysShowScrollbar(true);

		for (int i = 0; i< ItemsArray.Num(); i++)
		{
			if (!HorizontalBox)
			{
				HorizontalBox = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
			}
			
			UMItemWidget* ItemWidget = CreateWidget<UMItemWidget>(GetOwningPlayer(), ItemStaticWidget);
			ItemWidget->SetItemInfo(ItemsArray[i]);
			HorizontalBox->AddChild(ItemWidget);
			Count = Count + 1;
			
			if (Count == CountOfItemInOneRow)
			{
				ScrollBox->AddChild(HorizontalBox);
				HorizontalBox = nullptr;
				Count = 0;
			}

			if (i == ItemsArray.Num() - 1 && Count < CountOfItemInOneRow && Count > 0)
			{
				ScrollBox->AddChild(HorizontalBox);
			}
		}
	}
	else
	{
		InformTextBlock->SetVisibility(ESlateVisibility::Visible);
		ScrollBox->SetAlwaysShowScrollbar(false);
	}
}
