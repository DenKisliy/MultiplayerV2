// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerWidget/Item/MItemWidget.h"
#include "../../../../Public/Character/MPlayerCharacter.h"

void UMItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RemoveButton)
	{
		RemoveButton->OnClicked.AddDynamic(this, &UMItemWidget::RemoveItem);
	}

	if (UseButton)
	{
		UseButton->OnClicked.AddDynamic(this, &UMItemWidget::UseItem);
	}
}

void UMItemWidget::SetItemInfo(UMItemBase* ItemData)
{
	if (IsValid(ItemData))
	{
		if (!ItemData->Descriotion.GetName().IsEmpty())
		{
			Description = ItemData->Descriotion;
		}
		else
		{
			Description.ItemName = "Potion";
		}
		Count = ItemData->GetCount();
		TypeInfo = ItemData->TypeInfo;
		
		ItemData->OnIconTextureLoadDelegate.Clear();
		ItemData->OnIconTextureLoadDelegate.BindDynamic(this, &UMItemWidget::SetIcon);
		if (ItemData->IconPath.Get())
		{
			SetIcon(ItemData->IconPath.Get());
		}
		else
		{
			ItemData->GetIconTexture();
		}

		if (IsValid(ItemData->GameplayEffect))
		{
			UseButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UseButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMItemWidget::SetIcon(UTexture2D* Texture)
{
	Image->SetBrushFromTexture(Texture);
	FString text = Description.GetName() + " : " + FString::FromInt(Count);
	TextBlock->SetText(FText::FromString(text));
}

void UMItemWidget::RemoveItem()
{
	if (APlayerController* playerController = Cast<APlayerController>(GetOwningPlayer()))
	{
		if (AMPlayerCharacter* Player = Cast<AMPlayerCharacter>(playerController->GetPawn()))
		{
			Player->InventoryComponent->RemoveItemFromInventory(TypeInfo);
		}
	}

	if (Count > 1)
	{
		Count = Count - 1;
		FString text = Description.GetName() + " : " + FString::FromInt(Count);
		TextBlock->SetText(FText::FromString(text));
	}
	else
	{
		this->RemoveFromParent();
	}
}

void UMItemWidget::UseItem()
{
	if (APlayerController* playerController = Cast<APlayerController>(GetOwningPlayer()))
	{
		if (AMPlayerCharacter* player = Cast<AMPlayerCharacter>(playerController->GetPawn()))
		{
			player->InventoryComponent->UseItem(TypeInfo);
		}
	}

	if (Count > 1)
	{
		Count = Count - 1;
		FString text = Description.GetName() + " : " + FString::FromInt(Count);
		TextBlock->SetText(FText::FromString(text));
	}
	else
	{
		this->RemoveFromParent();
	}
}
