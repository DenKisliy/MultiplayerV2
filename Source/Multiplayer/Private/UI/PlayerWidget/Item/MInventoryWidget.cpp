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
		int count = 0;
		UHorizontalBox* horizontalBox = nullptr;
		for (int i = 0; i< ItemsArray.Num(); i++)
		{
			if (!horizontalBox)
			{
				horizontalBox = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
			}
			
			UMItemWidget* itemWidget = CreateWidget<UMItemWidget>(GetOwningPlayer(), ItemStaticWidget);
			itemWidget->SetItemInfo(ItemsArray[i]);
			horizontalBox->AddChild(itemWidget);
			count = count + 1;
			
			if (count == CountOfItemInOneRow)
			{
				ScrollBox->AddChild(horizontalBox);
				horizontalBox = nullptr;
				count = 0;
			}

			if (i == ItemsArray.Num() - 1 && count < CountOfItemInOneRow && count> 0)
			{
				ScrollBox->AddChild(horizontalBox);
			}
		}
	}
}
